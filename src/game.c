//
// Copyright (c) 2024 Luiz Gabriel Moraes "Luiz".
//
// This file is part of SimpleChess_SDL.
//
// SimpleChess_SDL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 3
// as published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// SimpleChess_SDL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>..
//

#include "game.h"
#include "res.h"


#define FPS 60

void game()
{
    GameData game = (GameData){0};

    if(!init(&game.renderer, &game.window))
    {
        SDL_ShowSimpleMessageBox(
           SDL_MESSAGEBOX_INFORMATION,
           "Initialization ERROR",
           "Fail to Initializing Resources",
           game.window);
        quit(game.renderer, game.window);
        return;
    }

    //game.ui_elements = init_elements();

    /*if(game.ui_elements == NULL)
    {
        SDL_ShowSimpleMessageBox(
           SDL_MESSAGEBOX_INFORMATION,
           "Initialization ERROR",
           "Fail to Initializing Ui Elements",
           game.window);
        quit(game.renderer, game.window);
        return;
    }*/

    for (_Bool quit = 0; !quit;)
    {
        quit = event_proc();

        game.start_time = SDL_GetTicks();

        /*
        update_elements(game.ui_elements);

        ui_present(game.ui_elements);*
        */

        SDL_RenderPresent(game.renderer);

        game.frame_time = SDL_GetTicks() - game.start_time;

        if (game.frame_time < (1000 / FPS))
            SDL_Delay((1000 / FPS) - game.frame_time); // sleeps through the time remaining to keep the fps stable
    }

    //free_elements(game.ui_elements);
    quit(game.renderer, game.window);
}

_Bool event_proc()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        default:
        }
    }
    return false;
}