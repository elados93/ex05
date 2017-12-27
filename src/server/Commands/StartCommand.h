
// Created by Elad Aharon on 15/12/17.
// ID: 311200786
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

private:
    /**
     * this functions loop throught the rooms vector and checks if the name of the optional room allready
     * exists.
     * @param rooms is the rooms vector
     * @param name is the new optional room
     * @return true or false
     */
    bool checkIfRoomAlreadyExists(vector<Room*> &rooms, string name);
};


#endif //EX04_STARTCOMMAND_H
