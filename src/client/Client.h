
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#ifndef EX04_CLIENT_H
#define EX04_CLIENT_H

#include <iostream>

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

    int priority;
private:
    const char *serverIP;
    int serverPort;
    int clientSocket;

    void writeToServer(string request);
    string readFromServer();
};


#endif //EX04_CLIENT_H
