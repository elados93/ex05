
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#ifndef EX04_CLIENT_H
#define EX04_CLIENT_H

#include <iostream>
#include <Point.h>

using namespace std;

class Client {
public:
    Client(const char *serverIP, int serverPort);

    virtual ~Client();

    void connectToServer();

    /**
     * getting x&y legal values and sending them to the sever..
     * @param x Point value.
     * @param y Point value.
     */
    void sendPoint(int x, int y);

    /**
     * get priority according to the client socket.
     * @return the priority number.
     */
    int getPriority();

    /**
     * @return the specific client socket
     */
    int getClientSocket() const;

    void handleBeforeGame();

    /**
    * this function reads 2 integers from the server and translate it to a real move on the board.
    * playOneTurn func will execute the move.
    * Point lastMove - is getting update by the move of the other player.
    * @return 1 if reading a point was a success, or -1 in case the other client had no moves.
    */
    Point *translatePointFromServer();
    void writeToServer(string request);

    int priority;
private:
    const char *serverIP;
    int serverPort;
    int clientSocket;

    string readFromServer();
    string translateDetails(string s); // trim the first word from th string s
};


#endif //EX04_CLIENT_H
