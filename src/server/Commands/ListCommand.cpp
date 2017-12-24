
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#include "ListCommand.h"


ListCommand::ListCommand() {}

void ListCommand::execute(SocketAndInformation *args) {
    string listOfRooms;
    if (countUnActiveRooms(*(args->rooms)) == 0) {
        listOfRooms.append("There are no rooms!");
    } else {
        listOfRooms.append("The rooms for now are:");
        for (int i = 0; i < args->rooms->size(); ++i) {
            Room currentRoom = *(args->rooms->at(i));
            if (!currentRoom.isRunning) {
                listOfRooms.append(" ");
                listOfRooms.append(currentRoom.roomName);
            }
        }
    }


    // inform the client to the rooms list
    sendFeedbackToClient(listOfRooms, args->socketSrc);
}

int ListCommand::countUnActiveRooms(vector<Room *> &rooms) {
    int counter = 0;
    for (int i = 0; i < rooms.size(); ++i) {
        if (!rooms.at(i)->isRunning)
            counter++;
    }
    return counter;
}
