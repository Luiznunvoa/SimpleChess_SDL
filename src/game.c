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
#include "ui.h"
#include "input.h"

#define FPS 60

void game()
{
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;

    Uint32 start_time = 0; // The tick that the frame started
    Uint32 frame_time = 0; // The time that the frame took to be rendered

    if(!init(&renderer, &window))
    {
        alert("Critical Error: Failed to Initialize Window", window);
        quit(&renderer, &window);
        return;
    }

    if(!init_ui(&renderer))
    {
        alert("Critical Error: Failed to Initialize the UI", window);
        quit(&renderer, &window);
        return;
    }

    for (_Bool quit = 0; !quit;)
    {
        start_time = SDL_GetTicks();

        quit = event_proc();

        if(!UI())
        {
            alert("Critical Error: Failed to Update the UI", window);
            quit = true;
        }

        frame_time = SDL_GetTicks() - start_time;

        if (frame_time < (1000 / FPS))
            SDL_Delay((1000 / FPS) - frame_time); // sleeps through the time remaining to keep the fps stable
    }

    free_UI();
    quit(&renderer, &window);
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
        case SDL_KEYUP:
            key_input_proc(event.key.keysym.sym);
        default:
        }
    }
    return false;
}