
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#ifndef EX04_PLAYCOMMAND_H
#define EX04_PLAYCOMMAND_H

#include "Command.h"

class PlayCommand : public  Command {
public:
    virtual void execute(void * args);
};


#endif //EX04_PLAYCOMMAND_H
