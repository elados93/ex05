
// Created by Elad Aharon and Shahar Palmor 19/12/17.
// ID: 311200786

#include <unistd.h>
#include "Command.h"

void Command::sendFeedbackToClient(string request, int clientSocket) {
    unsigned long sLen = request.length();
    int n;
    n = (int) write(clientSocket, &sLen, sizeof(sLen)); // writing the length of the command
    if (n == -1)
        throw "Error writing string length!";
    for (int i = 0; i < sLen; i++) {
        n = (int) write(clientSocket, &request[i], sizeof(char)); // writing to the client the command it self.
        if (n == -1)
            throw "Error writing message!";
    }
}
