
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
    /**
     * this function get the information of the port.
     * @param fileName is the name of the file
     * @return the port nunmber.
     */
    static int getPortFromFile(string fileName);

    /**
     * make an accuess to the map of commands allowed to the clients.
     * @return the command manager.
     */
    CommandManager *getCommandManager() const;

    /**
     * checks if the last command of the client was to create a room and after making the room close the
     * thread of this specific client.
     * @param command is the start command that verify if we can close this thread
     * @return true or false.
     */
    bool shouldThreadDie(string command);

    /**
     * checks if the last command of the client was to join to a valid room, and if so, starting a valid game room
     * on the thread of the joining client.
     * @param command is the join command if true.
     * @param mySocket the socket of the current player.
     * @return true or false.
     */
    bool shouldThreadRunGame(string command, int mySocket);

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

    /**
     * delete a current thread running in case of starting a room or finishing a game.
     * @param threadToDelete
     */
    void deleteThread(pthread_t *threadToDelete);

    int getNumberOfConnectedClients() const;

    int getServerSocket() const;

    const vector<pthread_t *> &getVectorThreads() const;


private:
    int port;
    int serverSocket; // the socket's file descriptor
    CommandManager *commandManager;
    vector <pthread_t *> vectorThreads;
    vector <struct Room*>rooms;

    void closeAllRooms();
};

#endif //EX04_SERVER_H
