
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#ifndef EX04_COMMANDMANAGER_H
#define EX04_COMMANDMANAGER_H


#include <map>
#include "Command.h"

typedef struct SocketAndInformation {
    int socket;
    string information;
};

class CommandManager {
public:
    CommandManager();
    ~CommandManager();

    void executeCommand(string command, int socket);

private:
    map<string, Command *> mapCommands;
};


#endif //EX04_COMMANDMANAGER_H
