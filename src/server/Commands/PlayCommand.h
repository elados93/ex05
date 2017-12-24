
// Created by Elad Aharon on 16/12/17.
// ID: 311200786
//

#ifndef EX04_PLAYCOMMAND_H
#define EX04_PLAYCOMMAND_H

#include "Command.h"

class PlayCommand : public  Command {
public:
    PlayCommand();
    /**
     * the execute of the play function get an information that contains a string of the x and y value of the point and converts tham to
     * in values in order to send them to the translatePointFrom server that has a read function in Client.
     * @param args is the info of the x &y values.
     */
    virtual void execute(SocketAndInformation *args);

private:
    //int getTheOtherSocket(vector<struct Room*> &rooms, int socket);
};


#endif //EX04_PLAYCOMMAND_H
