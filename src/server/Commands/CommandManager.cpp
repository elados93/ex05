
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#include "CommandManager.h"
#include "StartCommand.h"
#include "CloseCommand.h"
#include "PlayCommand.h"
#include "JoinCommand.h"
#include "ListCommand.h"

CommandManager::CommandManager(vector <Room*> &room) : games(room) {
    mapCommands["start"] = new StartCommand();
    mapCommands["list_game"] = new ListCommand();
    mapCommands["join"] = new JoinCommand();
    mapCommands["play"] = new PlayCommand();
    mapCommands["close"] = new CloseCommand();
}

CommandManager::~CommandManager() {
    map<string, Command *>::iterator it;
    for (it = mapCommands.begin(); it != mapCommands.end(); it++)
        delete it->second; // Delete the command allocation
}

void CommandManager::executeCommand(string command, int socket) {
    unsigned long firstSpaceOccurrence = command.find_first_of(' ');
    string explicitCommand = command.substr(0,firstSpaceOccurrence);
    string information = command.substr(firstSpaceOccurrence, command.length());

    SocketAndInformation commandDetails;
    commandDetails.socket = socket;
    commandDetails.information = information;
    commandDetails.rooms = games;

    Command *commandObj = mapCommands[explicitCommand];
    commandObj->execute(commandDetails);
}
