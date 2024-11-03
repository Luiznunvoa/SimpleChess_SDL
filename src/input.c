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
#include "board.h"
#include "ui.h"

_Bool key_input_proc(const SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_ESCAPE:
        return true;
    case SDLK_UP:
        if(board_data.select_y > 0)
            board_data.select_y -= 1;
        break;
    case SDLK_DOWN:
        if(board_data.select_y < 7)
            board_data.select_y += 1;
        break;
    case SDLK_LEFT:
        if(board_data.select_x > 0)
            board_data.select_x -= 1;
        break;
    case SDLK_RIGHT:
        if(board_data.select_x < 7)
            board_data.select_x += 1;
        break;
    default:
    }
    refresh_ui();
    return false;
}