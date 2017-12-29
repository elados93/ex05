
// Created by Elad Aharon and Shahar Palmor 15/12/17.
// ID: 311200786 307929927
//

#ifndef EX04_COMMANDMANAGER_H
#define EX04_COMMANDMANAGER_H


#include <map>
#include "Command.h"
#include "../RoomsHandler.h"

using namespace std;

class Command;

struct SocketAndInformation {
    int socketSrc;
    int socketDst;
    string information;
    RoomsHandler *roomsHandler;
    Room* roomToDelete;
};

class CommandManager {
public:
    CommandManager(RoomsHandler &room);
    ~CommandManager();

    void executeCommand(string command, int socketSrc, int socketDst = 0, struct Room* roomToDelete = NULL);

private:
    map<string, Command *> mapCommands;
    RoomsHandler &games;
};


#endif //EX04_COMMANDMANAGER_H
