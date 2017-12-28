
// Created by Elad Aharon and Shahar Palmor 16/12/17.
// ID: 311200786
//

#include "CloseCommand.h"

void CloseCommand::execute(SocketAndInformation *args) {
    args->roomsHandler->closeRoom(args->roomToDelete);
    delete(args->roomToDelete);
}
