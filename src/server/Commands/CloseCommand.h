
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#ifndef EX04_CLOSECOMMAND_H
#define EX04_CLOSECOMMAND_H

#include "Command.h"
class CloseCommand : public Command {
public:
    virtual void execute(void * args);
};


#endif //EX04_CLOSECOMMAND_H
