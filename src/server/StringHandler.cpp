
// Created by Elad Aharon and Shahar Palmor 26/12/17.
// ID: 311200786 307929927
//

#include "StringHandler.h"

string StringHandler::getSubStringAfterSpace(string s) {
    unsigned long firstSpaceOccurrence = s.find_first_of(' ');
    string information;
    if (firstSpaceOccurrence < s.length())
        information = s.substr(firstSpaceOccurrence + 1, s.length()).c_str();
    return information;
}

string StringHandler::extractCommand(string s) {
    unsigned long firstSpaceOccurrence = s.find_first_of(' ');
    return s.substr(0, firstSpaceOccurrence); // extract
}



