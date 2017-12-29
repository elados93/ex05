
// Created by Elad Aharon and Shahar Palmor 16/12/17.
// ID: 311200786 307929927
//

#include <unistd.h>
#include "JoinCommand.h"

JoinCommand::JoinCommand() {}

void JoinCommand::execute(SocketAndInformation *args) {
    Room* wantedRoom = args->roomsHandler->getUnActiveRoom(args->information);
    
    if (wantedRoom != NULL) {
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

