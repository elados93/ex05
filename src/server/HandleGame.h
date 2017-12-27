
// Created by Elad Aharon on 23/12/17.
// ID: 311200786
//

#ifndef EX04_HANDLEGAME_H
#define EX04_HANDLEGAME_H


#include "Server.h"

class HandleGame {
public:
    HandleGame(Room &room, CommandManager &commandManager, pthread_mutex_t &mutex_lock);

    void handle2ClientsGame();
private:
    int socket1;
    int socket2;
    Room &room;
    CommandManager &commandManager;
    pthread_mutex_t &mutexLock;

    /**
     * handing a client request, the client is sending a move and the server give the other client the information.
     * @param clientSocketSrc is the client that wants to transfer the info.
     * @param clientSocketDsc is the client reciving the info.
     * @return 1 if the info was transfered correctly. 0 is the game is over.
     */
    int handleClient(int clientSocketSrc, int clientSocketDsc);
    bool checkValidCommand(string s);

};


#endif //EX04_HANDLEGAME_H
