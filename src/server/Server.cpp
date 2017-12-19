
// Created by Elad Aharon & Shahar Palmor.
// ID: 311200786 307929927

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cstdlib>


using namespace std;

Server::Server(int port) : port(port), serverSocket(0), commandManager(), numberOfConnectedClients(0) {

    cout << "Server was created" << endl;
}

void Server::start() {
    // Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        throw "Error opening socket";

    // Assign a local address to the socket
    struct sockaddr_in serverAddress;
    bzero((void *) &serverAddress,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr
    *) &serverAddress, sizeof(serverAddress)) == -1)
        throw "Error on binding";

    // Start listening to incoming connections
    listen(serverSocket, MAX_CONNECTED_CLIENTS);
    // Define the client socket's structures
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen;

    while (true) {
        cout << "Waiting for client..." << endl;
        // Accept a new client connection
        int clientSocket = accept(serverSocket, (struct
                sockaddr *) &clientAddress, &clientAddressLen);
        cout << "Client connected" << endl;
        if (clientSocket == -1)
            throw "Error on accept client #1";

        // check if there is enough space for another client
        if (numberOfConnectedClients != MAX_CONNECTED_CLIENTS) {

        int rc = pthread_create(&clientsThreads[numberOfConnectedClients], NULL
                , communicateWithClient, (void *)clientSocket);

        // check if the thread is valid
        if (rc) {
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }

        numberOfConnectedClients++; // increment the number of clients for the current one.
        }


        /*giveClientPriority(clientSocket1, clientSocket2); // inform the client's priority

        while (true) { // Play the game with 2 players

            if (handleClient(clientSocket1, clientSocket2, 1) != 1)
                break;
            if (handleClient(clientSocket2, clientSocket1, 2) != 1)
                break;
        }
*/

    } // end of listening to clients

}

void * Server :: communicateWithClient(void *socket) {
    int clientSocket = (int) socket;
    while (true) {
        int stringLength, n;
        n = (int) read(clientSocket, &stringLength, sizeof(int));

        char *command = new char[stringLength];
        for (int i = 0; i < stringLength; i++) {
            n = (int) read(clientSocket, &command[i], sizeof(char));
            if (n == -1)
                throw "Error reading message!";
        }
        string strCommand(command);
        delete(command);

        commandManager.executeCommand(strCommand, clientSocket);
    }

}

// Handle requests from a specific client
int Server::handleClient(int clientSocketSrc, int clientSocketDst, int srcPriority) {
    int xValue, yValue;

    // Read new move arguments from Src client.
    int n = read(clientSocketSrc, &xValue, sizeof(xValue));
    if (n == -1)
        throw "Error reading x value from Src client";

    if (n == 0) {
        cout << "Client disconnected" << endl;
        return -1;
    }
    if (xValue == -1) {
        cout << "Client " << srcPriority << " didn't played" << endl;
        n = write(clientSocketDst, &xValue, sizeof(xValue));
        if (n == -1)
            throw "Can't write no possible moves to Dst client";

        return 1;
    }

    // -2 signify signal to close all the connections if the game ended
    if (xValue == -2) {
        int winner = 3 - srcPriority; // Calculate the winner priority
        cout << "End of game! The winner is: Player #" << winner << endl;
        close(clientSocketSrc);
        close(clientSocketDst);
        return 0; // return 0 to signify end of game
    }

    // Get the y value
    n = read(clientSocketSrc, &yValue, sizeof(yValue));
    if (n == -1)
        throw "Error reading y value from Src client";

    cout << "Got move: " << (xValue + 1) << ", " << (yValue + 1) <<
                                                                       " From Player #" << srcPriority << endl;
    // Write back to the other client.
    n = write(clientSocketDst, &xValue, sizeof(xValue));
    if (n == -1)
        throw "Error writing x value back to Dst client";

    n = write(clientSocketDst, &yValue, sizeof(yValue));
    if (n == -1)
        throw "Error reading y value from Dst client";

    return 1;
}

void Server::stop() {
    close(serverSocket);
}

void Server::giveClientPriority(int socket1, int socket2) {
    // Write the priority of the #1 client
    int priority1 = 1;
    int n1 = write(socket1, &priority1, sizeof(priority1));
    if (n1 == -1) {
        cout << "Error writing to #1 socket" << endl;
        return;
    }

    // Write the priority of the #2 client
    int priority2 = 2;
    n1 = write(socket2, &priority2, sizeof(priority2));
    if (n1 == -1) {
        cout << "Error writing to #2 socket" << endl;
        return;
    }
}

int Server :: getPortFromFile(string fileName) {
    try {
        const char *serverSettingsFileName = fileName.c_str();
        ifstream fileInput(serverSettingsFileName);
        if (fileInput == NULL)
            perror("Error while open the server settings file");

        string portString;
        getline(fileInput, portString);
        portString = portString.replace(0, sizeof("port = ") - 1, "");
        stringstream stringstream1(portString);
        int port = 0;
        stringstream1 >> port;
        return port;
    } catch (char* ex) {
        cout << "error while reading settings file";
        exit(-1);
    }
}

