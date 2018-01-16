
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927
//

#include "Client.h"
#include "../server/StringHandler.h"
#include <stdio.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

using namespace std;

Client::Client(const char *serverIP, int serverPort) :
        serverIP(serverIP), serverPort(serverPort),
        clientSocket(0), priority(0) {}

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
}

bool Client::sendPoint(int x, int y) {
    try {
        stringstream streamX;
        streamX << x;
        string strX = streamX.str();
        stringstream streamY;
        streamY << y;
        string strY = streamY.str();

        string playCommand;
        playCommand.append("play ");
        playCommand.append(strX);
        playCommand.append(" ");
        playCommand.append(strY);

        unsigned long xLen = playCommand.length();
        int n1;

        n1 = (int) write(clientSocket, &xLen, sizeof(xLen));
        if (n1 == -1)
            return false;
        for (unsigned long i = 0; i < xLen; ++i) {
            n1 = (int) write(clientSocket, &playCommand[i], sizeof(char));
            if (n1 == -1)
                return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

int Client::getPriority() {
    int result;
    int n = read(clientSocket, &result, sizeof(result)); // result is holding the priority given by the server.
    if (n == -1)
        throw "Error reading priority from server";

    if (result != 1 && result != 2)
        return -1; // return -1 as a failure to reach server

    priority = result;
    return priority;
}

Client::~Client() {
    delete (serverIP);
}

bool Client::handleBeforeGame() {
    string request;
    while (true) {

        getline(cin, request); // get the request from client

        try {
            connectToServer();
        } catch (const char *msg) {
            cout << "Failed to connect to server. Reason: " << msg << endl;
            return false;
        }

        if (!checkCommandValidation(request)) {
            cout << "This is not a valid request!" << endl;
            continue;
        }

        if (!writeToServer(request)) // send the request to the server
            return false;

        string serverFeedback = readFromServer();

        if (strcmp(serverFeedback.c_str(), "Started") == 0) {
            string roomName = StringHandler::getSubStringAfterSpace(request);
            cout << "The room: " << roomName << " was created!" << endl;
            break;
        }

        if (strcmp(serverFeedback.c_str(), "Joined") == 0) {
            string roomToJoin = StringHandler::getSubStringAfterSpace(request);
            cout << "Joined: " << roomToJoin << endl;
            break;
        }

        cout << serverFeedback << endl; // print the list of the current rooms in the server
    }
    return true;
}

bool Client::writeToServer(string request) {
    unsigned long sLen = request.length();
    int n;

    try {
        n = (int) write(clientSocket, &sLen, sizeof(sLen));
        if (n == -1)
            throw "Error writing string length!";
        for (int i = 0; i < sLen; i++) {
            n = (int) write(clientSocket, &request[i], sizeof(char));
            if (n == -1)
                throw "Error writing message!";
        }
        return true; // in case of success return true.
    } catch (...) {
        return false; // in case of a failure return false.
    }
}

string Client::readFromServer() {
    unsigned long stringLength;
    int n;
    n = (int) read(clientSocket, &stringLength, sizeof(stringLength));
    if (n == 0)
        return "Server disconnected";
    if (n == -1)
        throw "Error reading string length!";

    char *command = new char[stringLength + 1];
    for (int i = 0; i < stringLength; i++) {
        n = (int) read(clientSocket, &command[i], sizeof(char));
        if (n == -1)
            throw "Error reading message!";
    }
    command[stringLength] = '\0';
    string strCommand(command);
    delete (command);
    return strCommand;
}

Point *Client::translatePointFromServer() {
    int xValue, yValue;

    // Read new move arguments from server.
    int n;
    n = (int) read(clientSocket, &xValue, sizeof(xValue));
    if (n == 0) // if server disconnected send the point -2 -2
        return new Point(-2, -2);
    if (n == -1)
        throw "Error reading x value from Src client";

    // Get the y value
    n = (int) read(clientSocket, &yValue, sizeof(yValue));
    if (n == -1)
        throw "Error reading y value from Src client";

    return new Point(xValue, yValue);
}

bool Client::checkCommandValidation(string s) {
    string command = StringHandler::extractCommand(s);
    return strcmp(command.c_str(), "start") == 0 || strcmp(command.c_str(), "list_games") == 0
           || strcmp(command.c_str(), "join") == 0;
}
