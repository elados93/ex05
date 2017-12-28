
// Created by Elad Aharon and Shahar Palmor 16/12/17.
// ID: 311200786
//

#include "ListCommand.h"


ListCommand::ListCommand() {}

void ListCommand::execute(SocketAndInformation *args) {
    string listOfRooms;
    if (args->roomsHandler->countUnActiveRooms() == 0) {
        listOfRooms.append("There are no rooms!");
    } else {
        listOfRooms = args->roomsHandler->listOfAllUnActiveRooms();
    }

    // inform the client to the rooms list
    sendFeedbackToClient(listOfRooms, args->socketSrc);
}


