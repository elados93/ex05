// Created by Shahar Palmor on 28/12/17.
// ID 307929927

#include <gtest/gtest.h>
#include "../server/RoomsHandler.h"

TEST(roomsHandlerTest, checkRoom){
    RoomsHandler roomsHandler;
    Room *room1 = new Room();
    Room *room2 = new Room();
    Room *room3 = new Room();
    Room *room4 = new Room();
    roomsHandler.createRoom(room1);
    roomsHandler.createRoom(room2);
    roomsHandler.createRoom(room3);

    room1->roomName = "eled";
    room4->roomName = "sheher";
    room4->isRunning = true;
    room4->socket2 = 3;

    int counter = roomsHandler.countUnActiveRooms();
    roomsHandler.createRoom(room4);
    EXPECT_TRUE(counter == 3);
    EXPECT_TRUE(roomsHandler.isRoomExists("eled"));
    EXPECT_TRUE(roomsHandler.isRoomExistsAndRunningWithSocket("sheher", 3));
    roomsHandler.closeAllRooms();
    EXPECT_TRUE(roomsHandler.areRoomsEmpty());
    EXPECT_FALSE(roomsHandler.getRunningRoom("eled"));

}