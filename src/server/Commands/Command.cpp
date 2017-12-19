
// Created by Elad Aharon on 19/12/17.
// ID: 311200786

#include <unistd.h>
#include "Command.h"

void Command::sendFeedbackToClient(string request, int clientSocket) {
    unsigned long sLen = request.length();
    int n;
    n = (int) write(clientSocket, &sLen, sizeof(int));
    if (n == -1)
        throw "Error writing string length!";
    for (int i = 0; i < sLen; i++) {
        n = (int) write(clientSocket, &request[i], sizeof(char));
        if (n == -1)
            throw "Error writing message!";
    }
}
