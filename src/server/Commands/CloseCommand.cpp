
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#include "CloseCommand.h"

void CloseCommand::execute(SocketAndInformation *args) {
    for(vector<struct Room*>::iterator it = args->rooms->begin(); it != args->rooms->end(); ++it) {
        if ((*it) == args->roomToDelete) {
            args->rooms->erase(it);
            break;
        }
    }
    delete(args->roomToDelete);
}
