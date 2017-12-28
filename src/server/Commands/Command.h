
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#ifndef EX04_COMMAND_H
#define EX04_COMMAND_H

#include <iostream>
#include "../Server.h"

using namespace std;

class Command {
public:
    /**
     * get aspecific command and follow the instruction of how to commit it.
     * @param args includes all the info needed to execure this command.
     */
    virtual void execute(struct SocketAndInformation *args) = 0;

    /**
     * disstructor
     */
    virtual ~Command() {};


    /**
     * return the result of the command execution, so that the client will  know how to deal with
     * the command. and so that he could decide if it was a valid command.
     * @param request was the result of the command
     * @param clientSocket is the socket of the client which we want to write to.
     */
    void sendFeedbackToClient(string request, int clientSocket);
};


#endif //EX04_COMMAND_H
