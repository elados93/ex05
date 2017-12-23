
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#include "StartCommand.h"

StartCommand::StartCommand() {}


void StartCommand::execute(SocketAndInformation *details) {
    
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
