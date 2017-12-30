
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927

#include "Server.h"
#include "HandleGame.h"
#include "StringHandler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

pthread_mutex_t mutex_lock;

using namespace std;

/**
 * this is the threadic function that deals with each connected client.
 * @param args is all the information needed to the clients connection.
 * @return
 */
void *communicateWithClient(void *args);

/**
 * this is the threadic function of the server
 * @param args is all the information needed to the servers connection.
 * @return
 */
void *acceptClients(void *args);


struct ClientSocketAndServer {
    Server *server;
    pthread_t *threadID;
    int clientSocket;
};

Server::Server(int port) : port(port), serverSocket(0), numberOfConnectedClients(0) {
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
    pthread_t *acceptClientsThread = new pthread_t();

    int rc1 = pthread_create(acceptClientsThread, NULL, acceptClients,
                            (void *) pointerToServer);
    // check if the thread is valid
    if (rc1) {
        cout << "Error: unable to create thread, " << rc1 << endl;
        exit(-1);
    }

    // check if there was an exit command
    while (true) {
        string command;
        getline(cin, command);
        if (strcmp(command.c_str(), "exit") == 0) {
            int cancelFeedback = pthread_cancel(*acceptClientsThread);
            if (cancelFeedback != 0) // check cancel function
                throw "Could not stop the accepting clients thread!";
            stop();
            delete(acceptClientsThread);
            break;
        }
    }

}

void *communicateWithClient(void *args) {
    struct ClientSocketAndServer *clientSocketAndServer = (struct ClientSocketAndServer *) args;
    int currentClientSocket = clientSocketAndServer->clientSocket;
    Server *server = clientSocketAndServer->server;
    pthread_t *current_thread = clientSocketAndServer->threadID;

    pthread_mutex_lock(&mutex_lock);
    server->numberOfConnectedClients++; // increment the number of clients for the current one.
    pthread_mutex_unlock(&mutex_lock);

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

        pthread_mutex_lock(&mutex_lock);
        server->getCommandManager()->executeCommand(strCommand, currentClientSocket);
        pthread_mutex_unlock(&mutex_lock);

        if (server->shouldThreadDie(strCommand)) { // if the command was start or list_games, stop the loop and delete the thread
            if (strcmp(strCommand.c_str(), "list_games") == 0) { // for list_games command close the socket
                close(currentClientSocket);
                pthread_mutex_lock(&mutex_lock);
                server->numberOfConnectedClients --;
                pthread_mutex_unlock(&mutex_lock);
            }
            break;
        }

        if (server->shouldThreadRunGame(strCommand,
                                        currentClientSocket)) { // if the command was join, the thread will now handle the game
            Room *gameRoom = server->getRoomFromCommand(strCommand); // find the current game room
            HandleGame currentGame(*gameRoom, *(server->getCommandManager()), mutex_lock);
            currentGame.handle2ClientsGame();

            pthread_mutex_lock(&mutex_lock);
            server->numberOfConnectedClients -= 2;
            pthread_mutex_unlock(&mutex_lock);

            break;
        }
    }
    server->deleteThread(current_thread); // delete the thread from the threads vector
    delete (clientSocketAndServer);
}

void *acceptClients(void *args) {
    Server *server = (Server *)args;
    int serverSocket = server->getServerSocket();
    int numberOfConnectedClients = server->getNumberOfConnectedClients();
    vector <pthread_t *> &vectorThreads = server->getVectorThreads();

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

        // check if there is enough space for another client
        if (numberOfConnectedClients != MAX_CONNECTED_CLIENTS) {

            pthread_t *currentThread = new pthread_t();
            pthread_mutex_lock(&mutex_lock);
            vectorThreads.push_back(currentThread);
            pthread_mutex_unlock(&mutex_lock);

            struct ClientSocketAndServer *clientSocketAndServer = new struct ClientSocketAndServer();
            clientSocketAndServer->server = (Server *)args;
            clientSocketAndServer->clientSocket = clientSocket;
            clientSocketAndServer->threadID = currentThread;

            int rc2 = pthread_create(currentThread, NULL, communicateWithClient,
                                     (void *) clientSocketAndServer);

            // check if the thread is valid
            if (rc2) {
                cout << "Error: unable to create thread, " << rc2 << endl;
                exit(-1);
            }
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

bool Server::shouldThreadDie(string command) {
    string explicitCommand = StringHandler::extractCommand(command);
    string roomName = StringHandler::getSubStringAfterSpace(command);
    
    if (strcmp(explicitCommand.c_str(), "list_games") == 0)
        return true;

    bool result = false;

    if (strcmp(explicitCommand.c_str(), "start") == 0) {
        pthread_mutex_lock(&mutex_lock);
        result = roomsHandler->isRoomExists(roomName);
        pthread_mutex_unlock(&mutex_lock);
    }
    return result;
}

Server::~Server() {
    delete (commandManager);
}

CommandManager *Server::getCommandManager() const {
    return commandManager;
}

bool Server::shouldThreadRunGame(string command, int mySocket) {
    string explicitCommand = StringHandler::extractCommand(command);
    string roomName = StringHandler::getSubStringAfterSpace(command);
    
    bool result = false;
    if (strcmp(explicitCommand.c_str(), "join") == 0) {
        pthread_mutex_lock(&mutex_lock);
        result = roomsHandler->isRoomExistsAndRunningWithSocket(roomName, mySocket);
        pthread_mutex_unlock(&mutex_lock);
    }
    return result;
}

Room *Server::getRoomFromCommand(string command) {
    string roomName = StringHandler::getSubStringAfterSpace(command);

    pthread_mutex_lock(&mutex_lock);
    Room *room = roomsHandler->getRunningRoom(roomName);
    pthread_mutex_unlock(&mutex_lock);
    return room;
}

void Server::deleteThread(pthread_t *threadToDelete) {
    pthread_mutex_lock(&mutex_lock);
    for (vector<pthread_t *>::iterator it = vectorThreads.begin(); it != vectorThreads.end(); ++it) {
        if (*it == threadToDelete) {
            vectorThreads.erase(it);
            delete (threadToDelete);
            pthread_mutex_unlock(&mutex_lock);
            return;
        }
    }
    pthread_mutex_unlock(&mutex_lock);
}

int Server::getNumberOfConnectedClients() const {
    return numberOfConnectedClients;
}

int Server::getServerSocket() const {
    return serverSocket;
}

vector<pthread_t *> &Server::getVectorThreads() {
    return vectorThreads;
}
