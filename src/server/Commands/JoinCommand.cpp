
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#include <unistd.h>
#include "JoinCommand.h"

JoinCommand::JoinCommand() {}

void JoinCommand::execute(SocketAndInformation *args) {
    int roomIndex = -1;
    for (int i = 0; i < args->rooms->size(); ++i) {
        Room *currentRoom = args->rooms->at(i);
        if (strcmp(currentRoom->roomName.c_str(), args->information.c_str()) == 0 && !currentRoom->isRunning) {
            roomIndex = i;
            break;
        }
    }
    
    if (roomIndex != -1) {
        Room *wantedRoom = args->rooms->at(roomIndex);
        wantedRoom->isRunning = true; // activating the room
        wantedRoom->socket2 = args->socketSrc; // assign the second player socket
        // sending priorities to players
        int priority1 = 1;
        int n = (int) write(wantedRoom->socket1, &priority1, sizeof(priority1));
        if (n == -1)
            throw "Error sending 1 to client 1";

        sendFeedbackToClient("Joined", args->socketSrc); // inform the client he joined the room

        int priority2 = 2;
        n = (int) write(wantedRoom->socket2, &priority2, sizeof(priority2));
        if (n == -1)
            throw "Error sending 2 to client 2";
    } else // There is not such room to be joint
        sendFeedbackToClient("There is no such room!", args->socketSrc);
}

