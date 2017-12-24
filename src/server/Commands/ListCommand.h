
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#ifndef EX04_LISTCOMMAND_H
#define EX04_LISTCOMMAND_H

#include "Command.h"

class ListCommand : public Command {
public:
    ListCommand();

    /**
     * the execute function loop throught the game rooms that currently occiupied with one client that opened the room.
     * it checks if the list is empty and if so returns that there are no room,
     * otherwise it concatinate the rooms to a string feedback that will be sent to the client in handleBeforeGame function
     * it uses the vector of rooms and uses the room game field in the socket and info struct.
     * @param args is information that is empty.
     */
    virtual void execute(SocketAndInformation *args);
};


#endif //EX04_LISTCOMMAND_H
