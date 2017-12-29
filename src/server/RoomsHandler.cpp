
// Created by Elad Aharon and Shahar Palmor
// ID: 311200786 307929927
//

#include "RoomsHandler.h"
#include <cstring>
#include <unistd.h>

using namespace std;

RoomsHandler::RoomsHandler() {}

int RoomsHandler::countUnActiveRooms() {
    int counter = 0;
    for (int i = 0; i < rooms.size(); ++i) {
        if (!rooms.at(i)->isRunning)
            counter++;
    }
    return counter;
}

bool RoomsHandler::isRoomExists(string roomName) {
    for (int i = 0; i < rooms.size(); ++i) {
        Room currentRoom = *rooms.at(i); // assign the current room
        // check if there is room with that name
        if (strcmp(currentRoom.roomName.c_str(), roomName.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

bool RoomsHandler::isRoomExistsAndRunningWithSocket (string roomName, int mySocket) {
    for (int i = 0; i < rooms.size(); ++i) {
        Room currentRoom = *rooms.at(i); // assign the current room
        // check if there is room with that name and if that room is occupied with 2 players
        if (strcmp(currentRoom.roomName.c_str(), roomName.c_str()) == 0 && currentRoom.isRunning
            && currentRoom.socket2 == mySocket) {
            return true;
        }
    }
    return false;
}

Room *RoomsHandler::getRunningRoom(string roomName) {
    for (int i = 0; i < rooms.size(); ++i) {
        Room *currentRoom = rooms.at(i); // assign the current room
        if (strcmp(currentRoom->roomName.c_str(), roomName.c_str()) == 0 && currentRoom->isRunning) {
            return currentRoom;
        }
    }
    return NULL;
}

void RoomsHandler::closeAllRooms() {
    for (int i = 0; i < rooms.size(); ++i) {
        Room *room = rooms.at(i);
        close(room->socket1);
        if (room->isRunning)
            close(room->socket2);
        delete(room);
    }
    rooms.clear();
}

bool RoomsHandler::areRoomsEmpty() {
    return rooms.empty();
}

void RoomsHandler::createRoom(Room *room) {
    rooms.push_back(room);
}

RoomsHandler::~RoomsHandler() {
    closeAllRooms();
}

string RoomsHandler::listOfAllUnActiveRooms() {
    string listOfRooms;
    listOfRooms.append("The rooms for now are:");
    for (int i = 0; i < rooms.size(); ++i) {
        Room currentRoom = *(rooms.at(i));
        if (!currentRoom.isRunning) {
            listOfRooms.append(" ");
            listOfRooms.append(currentRoom.roomName);
        }
    }
    return listOfRooms;
}

Room *RoomsHandler::getUnActiveRoom(string roomName) {
    for (int i = 0; i < rooms.size(); ++i) {
        Room *currentRoom = rooms.at(i);
        if (strcmp(currentRoom->roomName.c_str(), roomName.c_str()) == 0 && !currentRoom->isRunning) {
            return currentRoom;
        }
    }
    return NULL;
}

void RoomsHandler::closeRoom(Room *roomToDelete) {
    for(vector<struct Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
        if ((*it) == roomToDelete) {
           rooms.erase(it);
            return;
        }
    }
}
