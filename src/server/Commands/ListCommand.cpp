
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#include "ListCommand.h"

void ListCommand::execute(SocketAndInformation args) {
    char listOfRooms[24] = "The rooms for now are:";
    for (int i = 0; i < args.rooms.size(); ++i) {
        Room currentRoom = args.rooms.at(i);
        if (!currentRoom.isRunning) {
            strcat(listOfRooms, " ");
            strcat(listOfRooms, currentRoom.roomName.c_str());
        }
    }
}
