
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#ifndef EX04_SERVER_H
#define EX04_SERVER_H

#include <string.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "Commands/CommandManager.h"

#define MAX_CONNECTED_CLIENTS 10

using namespace std;

typedef struct Room {
    string roomName;
    int socket1 = NULL;
    int socket2 = NULL;
    bool isRunning = false;
};


class Server {
public:
    Server(int port);
    void start();
    void stop();

    static int getPortFromFile(string fileName);

private:
    int port;
    int serverSocket; // the socket's file descriptor
    int numberOfConnectedClients;
    CommandManager commandManager;
    pthread_t clientsThreads[MAX_CONNECTED_CLIENTS];
    vector rooms<Room*>;

    /**
     * handing a client request, the client is sending a move and the server give the other client the information.
     * @param clientSocketSrc is the client that wants to transfer the info.
     * @param clientSocketDsc is the client reciving the info.
     * @param srcPriority is the priority of the giving info client.
     * @return 1 if the info was transfered correctly. 0 is the game is over.
     */
    int handleClient(int clientSocketSrc, int clientSocketDsc, int srcPriority);

    /**
     * giving each client his priority from the server.
     * @param socket1 is the socket for the first player.
     * @param socket2 is the socket for the second player.
     */
    void giveClientPriority(int socket1, int socket2);

    void *communicateWithClient(void *socket); // thread function
};

#endif //EX04_SERVER_H
