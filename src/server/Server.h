
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#ifndef EX04_SERVER_H
#define EX04_SERVER_H

#include <string.h>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <vector>
#include "Commands/CommandManager.h"

#define MAX_CONNECTED_CLIENTS 10

using namespace std;

struct Room {
    string roomName;
    int socket1;
    int socket2;
    bool isRunning;
};

class CommandManager;

class Server {
public:
    Server(int port);

    virtual ~Server();

    void start();
    void stop();

    static int getPortFromFile(string fileName);

    CommandManager *getCommandManager() const;
    bool shouldThreadDie(string command);
    bool shouldThreadRunGame(string command);

    Room* getRoomFromCommand(string command);

    int numberOfConnectedClients;

    void deleteThread(pthread_t *threadToDelete);
private:
    int port;
    int serverSocket; // the socket's file descriptor
    CommandManager *commandManager;
    vector <pthread_t *> vectorThreads;
    vector <struct Room*>rooms;
};

#endif //EX04_SERVER_H
