#define SDL_MAIN_HANDLED

// NOTE: Gamepad name ID depends on drivers and OS
#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS "playstation"

#include "raylib.h"
#include "Game.h"


int main()
{
    // RUMBLE REQUIREMENT
    // SDL Init
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
    {
        TraceLog(LOG_ERROR, "SDL_Init(SDL_INIT_GAMECONTROLLER) : %s\n", SDL_GetError());
        return 1;
    }

    TraceLog(LOG_INFO, "SDL_Init(SDL_INIT_GAMECONTROLLER) Success\n");

    Game game;
    game.run();
    
    CloseWindow();              // Close window and OpenGL context

    return 0;
}
