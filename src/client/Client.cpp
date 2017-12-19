
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#include "Client.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

using namespace std;

Client::Client(const char *serverIP, int serverPort) :
        serverIP(serverIP), serverPort(serverPort),
        clientSocket(0), priority(0) {
    cout << "Client" << endl;
}

void Client::connectToServer() {
    // Create a socket point
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        throw "Error opening socket";
    }
    // Get a hostent structure for the given host address
    struct hostent *server;

    struct in_addr address;
    inet_pton(AF_INET, serverIP, &address);

    server = gethostbyaddr((const void *) &address, sizeof address, AF_INET);

    if (server == NULL) {
        throw "Host is unreachable";
    }
    // Create a structure for the server address
    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    memcpy((char *) &serverAddress.sin_addr.s_addr, (char *) server->h_addr, server->h_length);
    // htons converts values between host and network byte orders
    serverAddress.sin_port = htons(serverPort);
    // Establish a connection with the TCP server
    if (connect(clientSocket, (struct sockaddr
    *) &serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error connecting to server";
    }
    cout << "Connected to server" << endl << "Waiting for the other players..." << endl;
}

void Client::sendPoint(int x, int y) {
    // check if the wanted point is -1 -1 and the player has no moves.
    if (x == -1 && y == -1) {
        int n1 = write(clientSocket, &x, sizeof(x));
        if (n1 == -1)
            throw "Error writing x value to socket";
        return;
    }
    // tranfering the info from playOneturn to the server.
    int n1 = write(clientSocket, &x, sizeof(x));
    if (n1 == -1)
        throw "Error writing x value to socket";

    n1 = write(clientSocket, &y, sizeof(y));
    if (n1 == -1)
        throw "Error writing y value to socket";

}

int Client::getPriority() {
    int result;
    int n = read(clientSocket, &result, sizeof(result)); // result is holding the priority given by the server.
    if (n == -1)
        throw "Error reading priority from server";

    priority = result;
    return priority;
}

int Client::getClientSocket() const {
    return clientSocket;
}

Client::~Client() {
    delete(serverIP);
}

void Client::handleBeforeGame() {
    while(true) {
        string request;
        getline(cin, request); // get the request from client
        writeToServer(request); // send the request to the server

        string serverFeedback = readFromServer();

        if (strcmp(serverFeedback.c_str(), "Joined") == 0)
            break;

        if (strcmp(serverFeedback.c_str(), "Started") == 0) {
            unsigned long firstSpaceOccurrence = request.find_first_of(' ');
            string roomName = request.substr(firstSpaceOccurrence + 1, request.length());

            cout << "The room: " << roomName << " was created!" << endl;
            break;
        }

        cout << serverFeedback << endl; // print the list of the current rooms in the server
    }
}

void Client::writeToServer(string request) {
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

string Client::readFromServer() {
    int stringLength, n;
    n = (int) read(clientSocket, &stringLength, sizeof(int));
    if (n == -1)
        throw "Error reading string length!";
    char *command = new char[stringLength];
    for (int i = 0; i < stringLength; i++) {
        n = (int) read(clientSocket, &command[i], sizeof(char));
        if (n == -1)
            throw "Error reading message!";
    }
    string strCommand(command);
    delete(command);
    return strCommand;
}
