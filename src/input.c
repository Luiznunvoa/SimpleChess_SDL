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

// Process key inputs from the user
Uint32 key_input(const SDL_Keycode keycode, int* cursor_x, int* cursor_y, _Bool* piece_locked, _Bool* update)
{
    switch (keycode)
    {
    case SDLK_ESCAPE: // Input to quit game
        return 1; // QUIT_GAME

    case SDLK_UP: // Input to move the cursor up
        if(*cursor_y > 0)
            *cursor_y -= 1;
        *update = true;
        return 2; // SELECT_CELL

    case SDLK_DOWN: // Input to move the cursor down
        if(*cursor_y < 7)
            *cursor_y += 1;
        *update = true;
        return 2;

    case SDLK_LEFT: // Input to move the cursor left
        if(*cursor_x > 0)
            *cursor_x -= 1;
        *update = true;
        return 2;

    case SDLK_RIGHT: // Input to move the cursor right
        if(*cursor_x < 7)
            *cursor_x += 1;
        *update = true;
        return 2;

    case SDLK_f:
        *update = true;
        return 4; // DELETE_PIECE

    case SDLK_e:
        if(!*piece_locked)
        {
            *update = true;
            return 3; // LOCK_PIECE
        }
        *update = true;
        *piece_locked = false;
        return 0;

    case SDLK_RETURN:
        if(!*piece_locked)
        {
            *update = true;
            return 3; // LOCK_PIECE
        }
        *update = true;
        return 5; // MOVE_PIECE

    default:
        *update = true;
        return 0; // NOTHING
    }
}

// Process mouse inputs from de user
Uint32 mouse_input(int* cursor_x, int* cursor_y, int lock_x, int lock_y, _Bool* piece_locked, _Bool* update)
{
    int x, y;

    SDL_GetMouseState(&x, &y);

    x = (x - OFFSET_X) / CELL_W;
    y = (y - OFFSET_Y) / CELL_H;

    if (x >= 0 && y >= 0 && x < 8 && y < 8)
    {
        *update = true;

        if(*cursor_x == x && *cursor_y == y)
        {
            if(!*piece_locked)
                return 3;
            
            else if(lock_x == x && lock_y == y)
                *piece_locked = false;

            else
            {
                *cursor_x = x;
                *cursor_y = y;
                return 5;
            }
        }
        *cursor_x = x;
        *cursor_y = y;
        return 2;
    }
    return 0;
}