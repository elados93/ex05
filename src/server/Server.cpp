
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927

#include "Server.h"
#include "HandleGame.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

pthread_mutex_t mutex_lock;

using namespace std;

void *communicateWithClient(void *args);

struct ClientSocketAndServer {
    Server *server;
    pthread_t *threadID;
    int clientSocket;
};

Server::Server(int port) : port(port), serverSocket(0), numberOfConnectedClients(0) {
    commandManager = new CommandManager(rooms);
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
            vectorThreads.push_back(currentThread);

            struct ClientSocketAndServer *clientSocketAndServer = new struct ClientSocketAndServer();
            clientSocketAndServer->server = this;
            clientSocketAndServer->clientSocket = clientSocket;
            clientSocketAndServer->threadID = currentThread;

            int rc = pthread_create(currentThread, NULL, communicateWithClient,
                                    (void *) clientSocketAndServer);

            // check if the thread is valid
            if (rc) {
                cout << "Error: unable to create thread, " << rc << endl;
                exit(-1);
            }
        }

    } // end of listening to clients

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

        char *command = new char[stringLength];
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

        if (server->shouldThreadDie(strCommand)) // if the command was start, stop the loop and delete the thread
            break;

        if (server->shouldThreadRunGame(strCommand)) { // if the command was join, the thread will now handle the game
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

void Server::stop() {
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
    unsigned long firstSpaceOccurrence = command.find_first_of(' ');
    string explicitCommand = command.substr(0, firstSpaceOccurrence);
    string roomName = command.substr(firstSpaceOccurrence + 1, command.length());

    if (strcmp(explicitCommand.c_str(), "start") == 0) {
        pthread_mutex_lock(&mutex_lock);
        for (int i = 0; i < rooms.size(); ++i) {
            Room currentRoom = *rooms.at(i); // assign the current room
            // check if there is room with that name
            if (strcmp(currentRoom.roomName.c_str(), roomName.c_str()) == 0) {
                pthread_mutex_unlock(&mutex_lock);
                return true;
            }
        }
        pthread_mutex_unlock(&mutex_lock);
    }
    return false;
}

Server::~Server() {
    delete (commandManager);
}

CommandManager *Server::getCommandManager() const {
    return commandManager;
}

bool Server::shouldThreadRunGame(string command) {
    unsigned long firstSpaceOccurrence = command.find_first_of(' ');
    string explicitCommand = command.substr(0, firstSpaceOccurrence);
    string roomName = command.substr(firstSpaceOccurrence + 1, command.length());

    if (strcmp(explicitCommand.c_str(), "join") == 0) {
        pthread_mutex_lock(&mutex_lock);
        for (int i = 0; i < rooms.size(); ++i) {
            Room currentRoom = *rooms.at(i); // assign the current room
            // check if there is room with that name and if that room is occupied with 2 players
            if (strcmp(currentRoom.roomName.c_str(), roomName.c_str()) == 0 && currentRoom.isRunning) {
                pthread_mutex_unlock(&mutex_lock);
                return true;
            }
        }
        pthread_mutex_unlock(&mutex_lock);
    }
    return false;
}

Room *Server::getRoomFromCommand(string command) {
    unsigned long firstSpaceOccurrence = command.find_first_of(' ');
    string roomName = command.substr(firstSpaceOccurrence + 1, command.length());

    pthread_mutex_lock(&mutex_lock);
    for (int i = 0; i < rooms.size(); ++i) {
        Room *currentRoom = rooms.at(i); // assign the current room
        if (strcmp(currentRoom->roomName.c_str(), roomName.c_str()) == 0 && currentRoom->isRunning) {
            pthread_mutex_unlock(&mutex_lock);
            return currentRoom;
        }
    }
    pthread_mutex_unlock(&mutex_lock);
    return NULL;
}

void Server::deleteThread(pthread_t *threadToDelete) {
    pthread_mutex_lock(&mutex_lock);
    for(vector<pthread_t *>::iterator it = vectorThreads.begin(); it != vectorThreads.end(); ++it) {
        if ((*it) == threadToDelete) {
            vectorThreads.erase(it);
            delete(threadToDelete);
            pthread_mutex_unlock(&mutex_lock);
            return;
        }
    }
    pthread_mutex_unlock(&mutex_lock);
}
