
// Created by Elad Aharon and Shahar Palmor 26/12/17.
// ID: 311200786 307929927
//

#ifndef EX04_STRINGHANDLER_H
#define EX04_STRINGHANDLER_H

#include <string>

using namespace std;

class StringHandler {
public:
    /**
     * gets the string after the first space
     * @param s is all the string
     * @return the sub string
     */
    static string getSubStringAfterSpace(string s);

    /**
     * return the first part of the string until the space
     * @param s is all the string
     * @return the sub string.s
     */
    static string extractCommand(string s);
};


#endif //EX04_STRINGHANDLER_H
