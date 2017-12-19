
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
//

#include "StartCommand.h"

void StartCommand::execute(SocketAndInformation details) {

    Room *room = new Room();
    room->socket1 = details.socket; // This is the room creator socket
    room->socket2 = NULL; // Here will be the client who will join the room
    room->isRunning = false; // The game didn't start yet
    room->roomName = details.information; // Set the HAKALMARIM room

    details.rooms.push_back(room);

    // inform the client the room was created
    string feedBackToClient = "Started";
    sendFeedbackToClient(feedBackToClient, details.socket);
}
