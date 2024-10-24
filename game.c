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
#include "ui.h"
#include "board.h"

#define FPS 30

Uint32 last_frame_time;
Uint32 start_time;
Uint32 frame_time;
float delta_time;

SDL_Event event;

bool update;

bool game_init()
{
    if(!ui_init_elements())
        return false;

    update = true;
    return true;
}

void game_free()
{
    ui_free_elements();
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

        if(update)
            if(ui_update_elements())
                ui_present();
            else
                quit  = true;

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
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            if(!key_input_proc())
                return false;
        case SDL_KEYUP:
            break;
        default:
        }
    }
    return false;
}

bool key_input_proc()
{
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        return false;
    case SDLK_UP:
        if(board_data.select_y > 0)
            board_data.select_y -= 1;
        update = true;
        break;
    case SDLK_DOWN:
        if(board_data.select_y < 7)
            board_data.select_y += 1;
        update = true;
        break;
    case SDLK_LEFT:
        if(board_data.select_x > 0)
            board_data.select_x -= 1;
        update = true;
        break;
    case SDLK_RIGHT:
        if(board_data.select_x < 7)
            board_data.select_x += 1;
        update = true;
        break;
    default:
    }
    return true;
}