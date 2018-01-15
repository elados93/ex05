
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927

#include "Server.h"
#include "StringHandler.h"
#include "HandleClient.h"
#include "HandleGame.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace std;

pthread_mutex_t mutex;

void *communicateWithClient(void *args);

void *acceptClients(void *args);


Server::Server(int port) : port(port), serverSocket(0), numberOfConnectedClients(0) {
    pool = new ThreadPool(MAX_THREADS);
    roomsHandler = new RoomsHandler();
    commandManager = new CommandManager(*roomsHandler);
    cout << "Server was created" << endl;
}

void Server::start() {
    // Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        throw "Error opening socket";

    // Assign a local address to the socket
    struct sockaddr_in serverAddress;
    bzero((void *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr
    *) &serverAddress, sizeof(serverAddress)) == -1)
        throw "Error on binding";

    // create a thread that will accept clients
    Server *pointerToServer = this;

    Task *acceptClientsTask = new Task(acceptClients, (void *) pointerToServer);
    pool->addTask(acceptClientsTask);

    // check if there was an exit command
    while (true) {
        string command;
        getline(cin, command);
        if (strcmp(command.c_str(), "exit") == 0) {
            pool->terminate();
            delete(acceptClientsTask);
            stop();
            break;
        }
    }

}




void *communicateWithClient(void *args) {
    struct ClientSocketAndServer *clientSocketAndServer = (struct ClientSocketAndServer *) args;
    int currentClientSocket = clientSocketAndServer->clientSocket;
    Server *server = clientSocketAndServer->server;

    //pthread_mutex_t mutex = server->getMutex_lock();

    pthread_mutex_lock(&mutex);
    server->numberOfConnectedClients++; // increment the number of clients for the current one.
    pthread_mutex_unlock(&mutex);

    while (true) {
        unsigned long stringLength;
        int n;
        n = (int) read(currentClientSocket, &stringLength, sizeof(stringLength));
        if (n == -1)
            throw "Error reading string length";

        char *command = new char[stringLength + 1];
        for (int i = 0; i < stringLength; i++) {
            n = (int) read(currentClientSocket, &command[i], sizeof(char));
            if (n == -1)
                throw "Error reading message!";
        }
        command[stringLength] = '\0';
        string strCommand(command);
        delete (command);

        pthread_mutex_lock(&mutex);
        server->getCommandManager()->executeCommand(strCommand, currentClientSocket);
        pthread_mutex_unlock(&mutex);

        if (server->shouldTaskEnd(strCommand)) { // if the command was start or list_games, stop the loop and delete the thread
            if (strcmp(strCommand.c_str(), "list_games") == 0) { // for list_games command close the socket
                close(currentClientSocket); // close the connection with client.
                pthread_mutex_lock(&mutex);
                server->numberOfConnectedClients --; // decrease number of connected clients.
                pthread_mutex_unlock(&mutex);
            }
            break;
        }

        if (server->shouldTaskRunGame(strCommand,
                                      currentClientSocket)) { // if the command was join, the thread will now handle the game
            Room *gameRoom = server->getRoomFromCommand(strCommand); // find the current game room
            HandleGame currentGame(*gameRoom, *(server->getCommandManager()), mutex);
            currentGame.handle2ClientsGame();

            pthread_mutex_lock(&mutex);
            server->numberOfConnectedClients -= 2;
            pthread_mutex_unlock(&mutex);

            break;
        }
    }
    delete (clientSocketAndServer);
}

void *acceptClients(void *args) {
    Server *server = (Server *)args;
    int serverSocket = server->getServerSocket();
    //pthread_mutex_t mutex = server->getMutex_lock();

    // Start listening to incoming connections
    listen(serverSocket, MAX_CONNECTED_CLIENTS);
    // Define the client socket's structures
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen;
    while (true) {

        cout << "Waiting for client..." << endl;
        // Accept a new client connection
        int clientSocket = accept(serverSocket, (struct
                sockaddr *) &clientAddress, &clientAddressLen);
        cout << "Client connected" << endl;
        if (clientSocket == -1)
            throw "Error on accept client #1";

        pthread_mutex_lock(&mutex);
        int numberOfConnectedClients = server->getNumberOfConnectedClients();
        pthread_mutex_unlock(&mutex);
        // check if there is enough space for another client
        if (numberOfConnectedClients != MAX_CONNECTED_CLIENTS) {

            struct ClientSocketAndServer *clientSocketAndServer = new struct ClientSocketAndServer();
            clientSocketAndServer->server = (Server *)args;
            clientSocketAndServer->clientSocket = clientSocket;

            Task *communicateWithClientTask = new Task(communicateWithClient, (void *) clientSocketAndServer);
            server->getThreadPool()->addTask(communicateWithClientTask);
        }

    } // end of listening to clients
}














void Server::stop() {

    roomsHandler->closeAllRooms();
    while (true) {
        if (roomsHandler->areRoomsEmpty()) {
            break;
        }
    }
    close(serverSocket);
}

int Server::getPortFromFile(string fileName) {
    try {
        const char *serverSettingsFileName = fileName.c_str();
        ifstream fileInput(serverSettingsFileName);
        if (fileInput == NULL)
            perror("Error while open the server settings file");

        string portString;
        getline(fileInput, portString);
        portString = portString.replace(0, sizeof("port = ") - 1, "");
        stringstream stringstream1(portString);
        int port = 0;
        stringstream1 >> port;
        return port;
    } catch (char *ex) {
        cout << "error while reading settings file";
        exit(-1);
    }
}

bool Server::shouldTaskEnd(string command) {
    string explicitCommand = StringHandler::extractCommand(command);
    string roomName = StringHandler::getSubStringAfterSpace(command);
    
    if (strcmp(explicitCommand.c_str(), "list_games") == 0)
        return true;

    bool result = false;

    if (strcmp(explicitCommand.c_str(), "start") == 0) {
        pthread_mutex_lock(&mutex);
        result = roomsHandler->isRoomExists(roomName);
        pthread_mutex_unlock(&mutex);
    }
    return result;
}

Server::~Server() {
    delete (commandManager);
    delete (roomsHandler);
    delete(pool);
}


bool Server::shouldTaskRunGame(string command, int mySocket) {
    string explicitCommand = StringHandler::extractCommand(command);
    string roomName = StringHandler::getSubStringAfterSpace(command);
    
    bool result = false;
    if (strcmp(explicitCommand.c_str(), "join") == 0) {
        pthread_mutex_lock(&mutex);
        result = roomsHandler->isRoomExistsAndRunningWithSocket(roomName, mySocket);
        pthread_mutex_unlock(&mutex);
    }
    return result;
}

CommandManager *Server::getCommandManager() const {
    return commandManager;
}

Room *Server::getRoomFromCommand(string command) {
    string roomName = StringHandler::getSubStringAfterSpace(command);

    pthread_mutex_lock(&mutex);
    Room *room = roomsHandler->getRunningRoom(roomName);
    pthread_mutex_unlock(&mutex);
    return room;
}

int Server::getNumberOfConnectedClients() const {
    return numberOfConnectedClients;
}

int Server::getServerSocket() const {
    return serverSocket;
}

ThreadPool *Server::getThreadPool() {
    return pool;
}

/*pthread_mutex_t &Server::getMutex_lock() {
    return mutex_lock;
}*/
