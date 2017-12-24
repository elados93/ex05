
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#include "StartCommand.h"

StartCommand::StartCommand() {}


void StartCommand::execute(SocketAndInformation *details) {

    if (checkIfRoomAlreadyExists(*(details->rooms), details->information))
        sendFeedbackToClient("There is already room with that name!", details->socketSrc);
    else {
        Room *room = new Room();
        room->socket1 = details->socketSrc; // This is the room creator socket
        room->socket2 = -1; // Here will be the client who will join the room, -1 as NULL
        room->isRunning = false; // The game didn't start yet
        room->roomName = details->information; // Set the hakalmarim room

        details->rooms->push_back(room);

        // inform the client the room was created
        string feedBackToClient = "Started";
        sendFeedbackToClient(feedBackToClient, details->socketSrc);
    }
}

bool StartCommand::checkIfRoomAlreadyExists(vector<Room *> &rooms, string name) {
    for (int i = 0; i < rooms.size(); ++i) {
        Room *currentRoom = rooms.at(i);
        if (strcmp(currentRoom->roomName.c_str(), name.c_str()) == 0)
            return true;
    }
    return false;
}