
// Created by Elad Aharon and Shahar Palmor 16/12/17.
// ID: 311200786 307929927
//

#include "CloseCommand.h"

CloseCommand::CloseCommand() {}

void CloseCommand::execute(SocketAndInformation *args) {
    args->roomsHandler->closeRoom(args->roomToDelete);
    delete(args->roomToDelete);
}

