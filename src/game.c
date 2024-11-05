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

#define FPS 15

#include "game.h"
#include "input.h"
#include "res.h"
#include "ui.h"

void game()
{
    WindowContext res;

    if(!init(&res))
    {
        alert("Critical Error: Failed to Initialize Window", res.window);
        quit(&res);
        return;
    }

    UIContext ui;

    if(!init_ui(&ui, res.renderer))
    {
        alert("Critical Error: Failed to Initialize the UI", res.window);
        quit(&res);
        return;
    }

    for (_Bool quit = 0; !quit;)
    {
        res.start_time = SDL_GetTicks();

        quit = event_proc(&ui.update);


        while(ui.update)
        {
            ui.update = false;

            if(!update_ui(&ui))
            {
                alert("Critical Error: Failed to Update the UI", res.window);
                quit = true;
            }
            else
                present_ui(ui.elements,  res.renderer);
        }

        res.frame_time = SDL_GetTicks() - res.start_time;

        if (res.frame_time < (1000 / FPS))
            SDL_Delay((1000 / FPS) - res.frame_time); // sleeps through the time remaining to keep the fps stable
    }

    free_UI(ui.elements);
    quit(&res);
}

_Bool event_proc(_Bool* update)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYUP:
            *update = true;
            return key_input_proc(event.key.keysym.sym);
        default:
        }
    }
    return false;
}