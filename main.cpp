#include <iostream>
#include "game.h"

int main()
{
    Game game;
    GameMode gameMode;
    bool startGame = false;
    game.startPage(gameMode, startGame);
    if(startGame)
        game.go(gameMode);
    return 0;
}
