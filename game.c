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

#include <SDL.h>

#include "game.h"
#include "res.h"

#define FPS 15

Uint32 last_frame_time;
Uint32 start_time;
Uint32 frame_time;
float delta_time;

SDL_Event event;

bool update;

bool game_init()
{
    SDL_SetRenderDrawColor(renderer, 243, 238, 234, 1);

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    return true;
}

void game_free()
{

}

void game()
{
    for (bool quit = 0; !quit;)
    {
        start_time = SDL_GetTicks();
        delta_time = (float)(start_time - last_frame_time) / (1000.0f / FPS);

        if (delta_time > 1)
            delta_time = 1;

        quit = event_proc();

        /*
        if(update)
            if(update_elements())
                ui_present(&elements;);
         */

        frame_time = SDL_GetTicks() - start_time;

        if (frame_time < (1000 / FPS))
            SDL_Delay((1000 / FPS) - frame_time);

        last_frame_time = SDL_GetTicks();
    }
}

bool event_proc()
{
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            return true;
    }
    return false;
}
