
// Created by Elad Aharon and Shahar Palmor 16/12/17.
// ID: 311200786 307929927
//

#ifndef EX04_JOINCOMMAND_H
#define EX04_JOINCOMMAND_H

#include "Command.h"

class JoinCommand : public  Command {
public:
    JoinCommand();

    /**
     * the execute command of join first look in the vector of rooms if there is such room to join, and if so
     * update the values of the struct of the room so it will have the current client socket as the second socket and that the running
     * field will now be true.
     * it also give the two  socket there priority so that the clients will know which player are they
     * and it write to the handleBeforeGame function in client that a join command occure so that the function will be
     * finished.
     * @param args is the info holding the room to be joined.
     */
    virtual void execute(SocketAndInformation *args);
};


#endif //EX04_JOINCOMMAND_H
