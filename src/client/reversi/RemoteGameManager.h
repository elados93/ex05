
// Created by Elad Aharon & Shahar Palmor
// ID: 311200786 307929927
//

#ifndef EX04_REMOTEGAMEMANAGER_H
#define EX04_REMOTEGAMEMANAGER_H


#include "GameManager.h"
#include "../Client.h"

class RemoteGameManager : public GameManager {

public:
    RemoteGameManager(GameState &gameState, Player &player1, Player &player2, Printer &printer,
                      GameRules &gameRules, Client &client1);
    
    /**
     * giving the current player his priority according to his client socket.
     * @param playerNumber is the priority.
     */
    void setCurrentPlayer(int playerNumber);

    /**
     * run each game of each client.
     */
    void run();


private:
    /**
     * play a single turn of one client. notice that we will do two turns, one for each client.
     */
    void playOneTurn();

    Client *clientDetails;
    bool isServerStopped; // switch that signify if the server stopped and need to exit the game.

    /**
     * Gets a point from server and verify it.
     * @return -2 for ending the game, -1 for no moves 1 for success.
     */
    int verifyPoint();
};


#endif //EX04_REMOTEGAMEMANAGER_H
