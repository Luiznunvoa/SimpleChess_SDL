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
#include "ui.h"
#include "input.h"
#include "res.h"

void game()
{
    WindowContext res;

    if (!init(&res))
        quit(&res);

    GameContext game;

    init_game(&game);

    UIContext ui;

    if (!init_ui(&ui, res.renderer, game.board_map))
        quit(&res);

    while(true)
    {
        if (SDL_WaitEvent(&game.event))
        {
            if (!event_proc(&game, &ui.update))
                break;

            while (ui.update)
                if (update_ui(&ui, &game))
                    present_ui(ui.elements,  res.renderer);
                else
                    break;
        }
    }
    free_ui(ui.elements);
    quit(&res);
}

void init_game(GameContext* game)
{
    *game = (GameContext){0};

    const int _temp_board_map[8][8] = {
        {1, 2, 3, 4, 5, 6, 7, 8},
        {9, 10, 11, 12, 13, 14, 15, 16},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {17, 18, 19, 20, 21, 22, 23, 24},
        {25, 26, 27, 28, 29, 30, 31, 32}
    };

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            game->board_map[x][y] = _temp_board_map[x][y];
}

_Bool event_proc(GameContext* game, _Bool* update)
{
    switch (game->event.type)
    {
    case SDL_QUIT:
        return false;
    case SDL_KEYUP:
        return key_input_proc(game->event.key.keysym.sym, &game->cursor_x, &game->cursor_y, &game->delete, update);
    default:
    }
    return true;
}