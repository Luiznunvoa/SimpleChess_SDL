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
#include "ui.h"
#include "input.h"
#include "res.h"

void game()
{
    WindowContext res;

    if(!init(&res))
    {
        alert("Critical Error: Failed to Initialize Window", res.window);
        quit(&res);
        return;
    }

    GameContext game = (GameContext){0};
    {
        const int temp_board_map[8][8] = {
            {1, 2, 3, 4, 5, 6, 7, 8},
            {9, 10, 11, 12, 13, 14, 15, 16},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {17, 18, 19, 20, 21, 22, 23, 24},
            {25, 26, 27, 28, 29, 30, 31, 32}
        };

       for(int y = 0; y < 8; y++)
           for(int x = 0; x < 8; x++)
               game.board_map[x][y] = temp_board_map[x][y];
    }

    UIContext ui;

    if(!init_ui(&ui, res.renderer, game.board_map))
    {
        alert("Critical Error: Failed to Initialize the UI", res.window);
        quit(&res);
        return;
    }

    for (_Bool quit = 0; !quit;)
    {
        res.start_time = SDL_GetTicks();

        quit = event_proc(&ui.update, &game);

        while(ui.update)
        {
            ui.update = false;

            if(!update_ui(&ui, &game))
            {
                alert("Critical Error: Failed to Update the UI", res.window);
                quit = true;
            }
            else
                present_ui(ui.elements,  res.renderer);
        }

        res.frame_time = SDL_GetTicks() - res.start_time;

        if (res.frame_time < (1000 / FPS))
            SDL_Delay((1000 / FPS) - res.frame_time);
    }

    free_UI(ui.elements);
    quit(&res);
}

_Bool event_proc(_Bool* update, GameContext* game)
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
            return key_input_proc(event.key.keysym.sym, &game->cursor_x, &game->cursor_y, &game->delete);
        default:
        }
    }
    return false;
}