
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#include "JoinCommand.h"

void JoinCommand::execute(SocketAndInformation args) {
    int roomIndex = -1;
    for (int i = 0; i < args.rooms.size(); ++i) {
        Room currentRoom = args.rooms.at(i);
        if (strcmp(currentRoom.roomName.c_str(), args.information.c_str()) == 0) {
            roomIndex = i;
            break;
        }
    }

    // There is not such room to be joint!
    if (roomIndex != -1)
        sendFeedbackToClient("Joined", args.socket);
    else
        sendFeedbackToClient("There is no such room!", args.socket);
}
