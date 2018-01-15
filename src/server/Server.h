
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
#include "RoomsHandler.h"
#include "ThreadPool.h"

#define MAX_CONNECTED_CLIENTS 30 // can be modified by the programmer
#define MAX_THREADS 5

using namespace std;

class CommandManager;

class Server {
public:
    Server(int port);

    virtual ~Server();

    void start();
    void stop();
    /**
     * this function get the information of the port.
     * @param fileName is the name of the file
     * @return the port number.
     */
    static int getPortFromFile(string fileName);

    /**
     * make an accuess to the map of commands allowed to the clients.
     * @return the command manager.
     */
    CommandManager *getCommandManager() const;

    /**
     * checks if the last command of the client was to create a room and after making the room close the
     * task of this specific client.
     * @param command is the start command that verify if we can close this thread
     * @return true or false.
     */
    bool shouldTaskEnd(string command);

    /**
     * checks if the last command of the client was to join to a valid room, and if so, starting a valid game room
     * on the task of the joining client.
     * @param command is the join command if true.
     * @param mySocket the socket of the current player.
     * @return true or false.
     */
    bool shouldTaskRunGame(string command, int mySocket);

    /**
     * loop throught the vector of rooms and checks for a specific room, comming from the command param.
     * @param command is the room itself.
     * @return the pointer to the room.
     */
    Room* getRoomFromCommand(string command);

    /**
     * public member that make sure we are in the limit of the max client connected.
     */
    int numberOfConnectedClients;

    int getNumberOfConnectedClients() const;

    int getServerSocket() const;

    ThreadPool *getThreadPool();

    pthread_mutex_t &getMutex_lock();

private:
    int port;
    int serverSocket; // the socket's file descriptor
    CommandManager *commandManager;
    ThreadPool *pool;
    RoomsHandler *roomsHandler;

    //pthread_mutex_t mutex_lock;
};

#endif //EX04_SERVER_H
