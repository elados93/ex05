
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#ifndef EX04_COMMANDMANAGER_H
#define EX04_COMMANDMANAGER_H


#include <map>
#include "Command.h"

using namespace std;

class Command;

struct SocketAndInformation {
    int socketSrc;
    int socketDst;
    string information;
    vector <struct Room*>*rooms;
    Room* roomToDelete;
};

class CommandManager {
public:
    CommandManager(vector <struct Room*> &room);
    ~CommandManager();

    void executeCommand(string command, int socketSrc, int socketDst = 0, Room* roomToDelete = NULL);

private:
    map<string, Command *> mapCommands;
    vector<Room*> &games;
};


#endif //EX04_COMMANDMANAGER_H
