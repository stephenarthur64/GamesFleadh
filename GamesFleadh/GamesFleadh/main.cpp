
#include "raylib.h"
#include "Game.h"



int main()
{
    Game game;
    game.run();
    
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
