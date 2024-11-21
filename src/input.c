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

#include "input.h"

Uint32 key_input_proc(const SDL_Keycode keycode, int* cursor_x, int* cursor_y, _Bool* piece_locked, _Bool* update)
{
    switch (keycode)
    {
    case SDLK_ESCAPE:
        return 1;

    case SDLK_UP:
        if(*cursor_y > 0)
            *cursor_y -= 1;
        *update = true;
        return 2;

    case SDLK_DOWN:
        if(*cursor_y < 7)
            *cursor_y += 1;
        *update = true;
        return 2;

    case SDLK_LEFT:
        if(*cursor_x > 0)
            *cursor_x -= 1;
        *update = true;
        return 2;

    case SDLK_RIGHT:
        if(*cursor_x < 7)
            *cursor_x += 1;
        *update = true;
        return 2;

    case SDLK_f:
        *update = true;
        return 4; // DELETE_PIECE

    case SDLK_e: // LOCK_PIECE
        if(!*piece_locked)
        {
            *update = true;
            return 3;
        }
        *update = true;
        *piece_locked = false;

    default:
        return 0; // NOTHING
    }
}

Uint32 mouse_input_proc(int* cursor_x, int* cursor_y)
{
    int x, y;

    SDL_GetMouseState(&x, &y);

    x = (x - OFFSET_X) / CELL_W;
    y = (y - OFFSET_Y) / CELL_H;

    if (x >= 0 && y >= 0 && x < 8 && y < 8)
    {
        if(*cursor_x == x && *cursor_y == y)
            return 3;

        *cursor_x = x;
        *cursor_y = y;

        return 2;
    }
    return 0;
}