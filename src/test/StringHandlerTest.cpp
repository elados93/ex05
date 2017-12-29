
// Created by Elad Aharon and Shahar Palmor
// ID: 311200786 307929927
//

#include <gtest/gtest.h>
#include "../server/StringHandler.h"

/**
 * test the stringHandler class.
 */
TEST(StringHandlerTest,CheckSubStrings){
    StringHandler stringHandler;
    string s = "start hakalmarim";
    string command = stringHandler.extractCommand(s);
    string roomName = stringHandler.getSubStringAfterSpace(s);

    EXPECT_TRUE(strcmp("start",command.c_str()) == 0);
    EXPECT_TRUE(strcmp("hakalmarim",roomName.c_str()) == 0);
}

TEST(StringHandlerTest,CheckSubStringsEmpty){
    StringHandler stringHandler;
    string s = "";
    string command = stringHandler.extractCommand(s);
    string roomName = stringHandler.getSubStringAfterSpace(s);

    EXPECT_TRUE(strcmp("",command.c_str()) == 0);
    EXPECT_TRUE(strcmp("",roomName.c_str()) == 0);
}

TEST(StringHandlerTest,CheckSubStringsWithNoInfo){
    StringHandler stringHandler;
    string s = "start";
    string command = stringHandler.extractCommand(s);
    string roomName = stringHandler.getSubStringAfterSpace(s);

    EXPECT_TRUE(strcmp("start",command.c_str()) == 0);
    EXPECT_TRUE(strcmp("",roomName.c_str()) == 0);
}