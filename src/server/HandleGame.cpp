
// Created by Elad Aharon on 23/12/17.
// ID: 311200786
//

#include <unistd.h>
#include "HandleGame.h"

HandleGame::HandleGame(Room &room, CommandManager &commandManager, pthread_mutex_t &mutex_lock) : room(room),
                                                                                                  commandManager(
                                                                                                          commandManager),
                                                                                                  socket1(room.socket1),
                                                                                                  socket2(room.socket2),
                                                                                                  mutexLock(
                                                                                                          mutex_lock) {}

void HandleGame::handle2ClientsGame() {

    while (true) { // Play the game with 2 players

        if (handleClient(socket1, socket2) != 1)
            break;
        if (handleClient(socket2, socket1) != 1)
            break;
    }
}

// Handle requests from a specific client
int HandleGame::handleClient(int clientSocketSrc, int clientSocketDst) {

    unsigned long stringLength;
    int n;
    n = (int) read(clientSocketSrc, &stringLength, sizeof(stringLength));
    if (n == -1)
        throw "Error reading string length!";
    char *moveFromSrc = new char[stringLength];
    for (unsigned long i = 0; i < stringLength; i++) {
        n = (int) read(clientSocketSrc, &moveFromSrc[i], sizeof(char));
        if (n == -1)
            throw "Error reading message!";
    }
    moveFromSrc[stringLength] = '\0';

    string strCommand(moveFromSrc);
    delete (moveFromSrc);

    string explicitCommand = extractCommand(strCommand);
    if(!checkValidCommand(explicitCommand)){
        cout << " this is not a valid move!" << endl;
    }

    if (strcmp(explicitCommand.c_str(), "close") == 0) {
        strCommand.append(room.roomName);

        pthread_mutex_lock(&mutexLock);
        commandManager.executeCommand(strCommand, clientSocketSrc, clientSocketDst, &room); // do close command
        pthread_mutex_unlock(&mutexLock);

        return -1; // return -1 in order to signify end of game
    }

    pthread_mutex_lock(&mutexLock);
    commandManager.executeCommand(strCommand, clientSocketSrc, clientSocketDst); // do play command
    pthread_mutex_unlock(&mutexLock);

    return 1; // return 1 as a successful move operation
}

bool HandleGame::checkValidCommand(string s) {
    string command = extractCommand(s);
    if (strcmp(command.c_str(), "play") == 0 || strcmp(command.c_str(),"close"))
        return true;
    return false;
}

string HandleGame::extractCommand(string s) {
    unsigned long firstSpaceOccurrence = s.find_first_of(' ');
    return s.substr(0, firstSpaceOccurrence); // extract
}
