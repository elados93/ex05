
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#ifndef EX04_LISTCOMMAND_H
#define EX04_LISTCOMMAND_H

#include "Command.h"

class ListCommand : public Command {
public:
    ListCommand();
    virtual void execute(SocketAndInformation *args);
};


#endif //EX04_LISTCOMMAND_H
