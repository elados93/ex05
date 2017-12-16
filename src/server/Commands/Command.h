
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#ifndef EX04_COMMAND_H
#define EX04_COMMAND_H

#include <iostream>

using namespace std;

class Command {
public:
    virtual void execute(void *args) = 0;
    virtual ~Command() {};
protected:
    vector rooms

};

#endif //EX04_COMMAND_H
