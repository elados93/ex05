
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#ifndef EX04_COMMANDMANAGER_H
#define EX04_COMMANDMANAGER_H


#include <map>
#include "Command.h"

using namespace std;

typedef struct SocketAndInformation {
    int socket;
    string information;
    vector &rooms<Room*>;
};

class CommandManager {
public:
    CommandManager(vector <Room*> &room);
    ~CommandManager();

    void executeCommand(string command, int socket);

private:
    map<string, Command *> mapCommands;
    vector<Room*> &games;
};


#endif //EX04_COMMANDMANAGER_H
