
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#ifndef EX04_STARTCOMMAND_H
#define EX04_STARTCOMMAND_H

#include "Command.h"
#include "CommandManager.h"

class StartCommand : public Command {
public:
    StartCommand();

    virtual void execute(SocketAndInformation *args);
};


#endif //EX04_STARTCOMMAND_H
