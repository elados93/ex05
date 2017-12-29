
// Created by Elad Aharon and Shahar Palmor
// ID: 311200786 307929927
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

    /***
     * loop through the rooms and finds all the rooms that are not running
     * @return the rooms that are not active
     */
    int countUnActiveRooms();

    /**
     * @param roomName is the specific room that we check if its an un active room
     * @return the pointer to the un active room
     */
    Room *getUnActiveRoom(string roomName);

    /**
     * loop through the rooms and returns if there are no rooms.
     * @return
     */
    bool areRoomsEmpty();

    /**
     * check if a room with the string "roomName" exists
     * @param roomName is the room we look
     * @return true or false
     */
    bool isRoomExists(string roomName);

    /**
     * check if a room with the string "roomName" exists, running and with that socket as
     * the second socket.
     * @param roomName is the room we look
     * @param mySocket is the second socket
     * @return true or false
     */
    bool isRoomExistsAndRunningWithSocket(string roomName, int mySocket);

    /**
     * @return a list of all un active rooms.
     */
    string listOfAllUnActiveRooms();

    /**
     * gets a name room and if it exists return a pointer.
     * @param roomName
     * @return the pointer to the room
     */
    Room *getRunningRoom(string roomName);

    /**
     * loop through the rooms and close them one by one
     */
    void closeAllRooms();

    /**
     * @param roomToDelete is the room we delete in this function
     */
    void closeRoom(Room *roomToDelete);

    /**
     * get a pointer to a room that was created in the start command and push it to the rooms vector.
     * @param room is a specific room
     */
    void createRoom(Room *room);
    
private:
    vector <struct Room*>rooms;
};


#endif //EX04_ROOMSHANDLER_H
