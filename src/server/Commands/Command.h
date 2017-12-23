
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
    virtual void execute(struct SocketAndInformation *args) = 0;
    virtual ~Command() {};

    void sendFeedbackToClient(string request, int clientSocket);
};


#endif //EX04_COMMAND_H
