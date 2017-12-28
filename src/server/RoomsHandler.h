
// Created by Elad Aharon and Shahar Palmor
// ID: 311200786
//

#ifndef EX04_ROOMSHANDLER_H
#define EX04_ROOMSHANDLER_H

#include <vector>
#include <string>

using namespace std;

struct Room {
    string roomName;
    int socket1;
    int socket2;
    bool isRunning;
};

class RoomsHandler {
public:
    RoomsHandler();

    virtual ~RoomsHandler();

    int countUnActiveRooms();
    Room *getUnActiveRoom(string roomName);
    
    bool areRoomsEmpty();
    bool isRoomExists(string roomName);
    bool isRoomExistsAndRunningWithSocket(string roomName, int mySocket);
    
    string listOfAllUnActiveRooms();
    
    Room *getRunningRoom(string roomName);
    
    void closeAllRooms();
    void closeRoom(Room *roomToDelete);
    void createRoom(Room *room);
    
private:
    vector <struct Room*>rooms;
};


#endif //EX04_ROOMSHANDLER_H
