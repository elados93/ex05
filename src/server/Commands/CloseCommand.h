
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#ifndef EX04_CLOSECOMMAND_H
#define EX04_CLOSECOMMAND_H

#include "Command.h"
class CloseCommand : public Command {
public:

    /**
     * the execute command of the close loop thrught the list of rooms and delete the room.
     * it does not! delete the thread
     * it also update the vector of rooms.
     * @param args is the name of the room to be deleted.
     */
    virtual void execute(SocketAndInformation *args);
};


#endif //EX04_CLOSECOMMAND_H
