
// Created by Elad Aharon and Shahar Palmor 15/12/17.
// ID: 311200786 307929927
//

#ifndef EX04_STARTCOMMAND_H
#define EX04_STARTCOMMAND_H

#include "Command.h"
#include "CommandManager.h"

class StartCommand : public Command {
public:
    StartCommand();

    /**
     * the execute command of the start opens a room and update the client socket.
     * it sends a massage to the handleBeforeGame function in client that a start command occured.
     * @param args is the name of the room to be opened.
     */
    virtual void execute(SocketAndInformation *args);
};


#endif //EX04_STARTCOMMAND_H
