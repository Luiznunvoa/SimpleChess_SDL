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

void key_input_proc(const SDL_Keycode keycode, int* cursor_x, int* cursor_y,  Uint32* flag)
{
    switch (keycode)
    {
    case SDLK_ESCAPE:
        *flag = 1;
        break;
    case SDLK_UP:
        if(*cursor_y > 0)
            *cursor_y -= 1;
        break;
    case SDLK_DOWN:
        if(*cursor_y < 7)
            *cursor_y += 1;
        break;
    case SDLK_LEFT:
        if(*cursor_x > 0)
            *cursor_x -= 1;
        break;
    case SDLK_RIGHT:
        if(*cursor_x < 7)
            *cursor_x += 1;
        break;
    case SDLK_e: // to delete piece NOTE for debug
        *flag = 4;
        break;
    case SDLK_f: // to lock a piece
        *flag = 3;
        break;
    default:
    }
}

void mouse_input_proc(int* cursor_x, int* cursor_y)
{
    int x, y;

    SDL_GetMouseState(&x, &y);

    const int offset = (600 - 520) / 2;

    x = (x - offset) / 65;
    y = (y - offset) / 65;

    if (x >= 0 && y >= 0 && x < 8 && y < 8)
    {
        *cursor_x = x;
        *cursor_y = y;
    }
}