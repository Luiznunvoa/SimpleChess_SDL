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

#ifndef GAME_H
#define GAME_H

#include "common.h"

typedef enum
{
    DEFAULT,
    QUIT_GAME,
    SELECT_PIECE,
    LOCK_PIECE,
    DELETE_PIECE,
} GameFlags;

typedef struct
{
    int cursor_x;
    int cursor_y;

    int locked_piece_x;
    int locked_piece_y;

    _Bool piece_locked;
    int board[8][8];

    GameFlags flag;

    SDL_Event event;
}GameContext;

void game();

#endif //GAME_H
