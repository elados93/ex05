
// Created by Elad Aharon and Shahar Palmor 16/12/17.
// ID: 311200786
//

#include <sstream>
#include <unistd.h>
#include "PlayCommand.h"

PlayCommand::PlayCommand() {}

void PlayCommand::execute(SocketAndInformation *args) {
    
    unsigned long firstSpaceOccurrence = args->information.find_first_of(' ');
    string xString(args->information.substr(0 , firstSpaceOccurrence)); //this is the x val in string representation
    string yString(args->information.substr(firstSpaceOccurrence + 1, args->information.length())); //this is the y val in string representation

    int x, y, n;
    stringstream xStream(xString), yStream(yString);
    xStream >> x;
    yStream >> y;

    int dstSocket = args->socketDst;
    n = (int) write(dstSocket, &x, sizeof(x));
    if (n == -1)
        throw "Error sending x value";

    n = (int) write(dstSocket, &y, sizeof(y));
    if (n == -1)
        throw "Error sending y value";
}
