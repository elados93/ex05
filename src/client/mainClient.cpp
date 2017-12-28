
// Created by Elad Aharon & Shahar Palmor
// ID: 311200786 307929927

#include "Client.h"
#include "reversi/Board.h"
#include "reversi/GameState.h"
#include "reversi/HumanPlayer.h"
#include "reversi/AIPlayer.h"
#include "reversi/ConsolePrinter.h"
#include "reversi/GameManager.h"
#include "reversi/RemoteGameManager.h"

/**
* get the port of the connection with the server from the file.
* @return the port.
*/
Client * getClientFromFile(string fileName);

using namespace std;

int main() {

    Board *board = new Board(4, 4); // ##%%366363CHANGE BACK!!#!@$!%^*%(%*%&#%!~
    GameState gameState1(board);
    ReversiDefaultRules *gameRules = new ReversiDefaultRules();

    HumanPlayer p1('x');
    AIPlayer pAI('o', *gameRules);
    HumanPlayer p3('o');

    ConsolePrinter *menuConsole = new ConsolePrinter(*board, p1, p3);
    menuConsole->printMenu(); // Print the 3 options to the client.
    delete (menuConsole);

    int inputTypeGame;
    //cin >> inputTypeGame;

    switch (3) { // ##%%366363CHANGE BACK!!#!@$!%^*%(%*%&#%!~
        case 1: {
            ConsolePrinter printer1(*board, p1, p3);
            GameManager game1(gameState1, p1, p3, printer1, *gameRules, false);
            game1.run(); // Run the game vs local human.
            break;
        }
        case 2: {

            ConsolePrinter printer2(*board, p1, pAI);
            GameManager game2(gameState1, p1, pAI, printer2, *gameRules, true);
            game2.run(); // Run the game vs Computer.
            break;
        }
        case 3: {
            Client *client = getClientFromFile("../settingsClient.txt");
            
            ConsolePrinter printer3(*board, p1, p3);
            client->handleBeforeGame();

            int priorityClient = client->getPriority();
            if (priorityClient == -1)
                break; // couldn't get a valid priority
            
            printer3.printInformingGameStarted(client->priority == 1 ? PLAYER_1 : PLAYER_2);
            RemoteGameManager game3(gameState1, p1, p3, printer3, *gameRules, *client);
            game3.setCurrentPlayer(priorityClient);

            game3.run();

            break;
        }

        default: {
            cout << "Enter a valid option! :(" << endl;
            main();
            break;
        }

    }

    delete (gameRules);

    return 0;
}
