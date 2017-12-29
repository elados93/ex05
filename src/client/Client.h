
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#ifndef EX04_CLIENT_H
#define EX04_CLIENT_H

#include <iostream>
#include "reversi/Point.h"

using namespace std;

class Client {
public:
    Client(const char *serverIP, int serverPort); // Constructor

    virtual ~Client(); // Destructor

    /**
     * Try to connect the server. Raise an error message.
     */
    void connectToServer();

    /**
     * getting x&y legal values and sending them to the sever..
     * @param x Point value.
     * @param y Point value.
     * @return true or false if succed.
     */
    bool sendPoint(int x, int y);

    /**
     * get priority according to the client socket.
     * @return the priority number.
     */
    int getPriority();

    /**
     * The method handle the clients requests. In case of an un valid message (not start, join or list_game)
     * the method will notice the client.
     * @return true if the operation went well, false if the server disconnected.
     */
    bool handleBeforeGame();

    /**
    * this function reads 2 integers from the server and translate it to a real move on the board.
    * playOneTurn func will execute the move.
    * Point lastMove - is getting update by the move of the other player.
    * @return 1 if reading a point was a success, or -1 in case the other client had no moves.
    */
    Point *translatePointFromServer();

    /**
     * Write a request to the server. can be play x,y or close. if the server disconnected return false.
     * @param request The wanted request to the server.
     * @return True or false if the message delivered.
     */
    bool writeToServer(string request);

    int priority;
private:
    const char *serverIP;
    int serverPort;
    int clientSocket;

    /**
     * Check if the command is valid before game. start or list_games or join.
     * @param s The command.
     * @return True if it's valid false otherwise.
     */
    bool checkCommandValidation(string s);

    /**
     * Read message from server and return it.
     * @return The message from server.
     */
    string readFromServer();

};


#endif //EX04_CLIENT_H
