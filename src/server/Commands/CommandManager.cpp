
// Created by Elad Aharon and Shahar Palmor 15/12/17.
// ID: 311200786
//

#include "CommandManager.h"
#include "StartCommand.h"
#include "CloseCommand.h"
#include "PlayCommand.h"
#include "JoinCommand.h"
#include "ListCommand.h"
#include "../StringHandler.h"

CommandManager::CommandManager(RoomsHandler &room) : games(room) {
    mapCommands["start"] = new StartCommand();
    mapCommands["list_games"] = new ListCommand();
    mapCommands["join"] = new JoinCommand();
    mapCommands["play"] = new PlayCommand();
    mapCommands["close"] = new CloseCommand();
}

CommandManager::~CommandManager() {
    map<string, Command *>::iterator it;
    for (it = mapCommands.begin(); it != mapCommands.end(); it++)
        delete (it->second); // Delete the command allocation
}

void CommandManager::executeCommand(string command, int socketSrc, int socketDst, Room* roomToDelete) {
    string explicitCommand = StringHandler::extractCommand(command);
    string information = StringHandler::getSubStringAfterSpace(command);

    SocketAndInformation *commandDetails = new SocketAndInformation();
    commandDetails->socketSrc = socketSrc;
    commandDetails->socketDst = socketDst; // only in play & close commands
    commandDetails->information = information;
    commandDetails->roomsHandler = &games;
    commandDetails->roomToDelete = roomToDelete; // only in close command

    Command *commandObj = mapCommands[explicitCommand];
    commandObj->execute(commandDetails);

    delete(commandDetails);
}
