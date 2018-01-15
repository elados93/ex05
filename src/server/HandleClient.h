
// Created by Elad Aharon on 14/01/18.
// ID: 311200786
//

#ifndef EX04_HANDLECLIENT_H
#define EX04_HANDLECLIENT_H

#include "Server.h"

struct ClientSocketAndServer {
    Server *server;
    int clientSocket;
};

class HandleClient {
public:

/**
 * this is the threadic function that deals with each connected client.
 * @param args is all the information needed to the clients connection.
 * @return
 */
    static void *communicateWithClient(void *args);

    /**
 * this is the threadic function of the server
 * @param args is all the information needed to the servers connection.
 * @return
 */
    static void *acceptClients(void *args);
};


#endif //EX04_HANDLECLIENT_H
