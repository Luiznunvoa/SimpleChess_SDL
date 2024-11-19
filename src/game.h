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
    DEFAULT,        // Nothing is happening
    QUIT_GAME,      // Exit the game
    SELECT_CELL,    // There's a cell been selected
    LOCK_PIECE,     // There's a piece been locked
    DELETE_PIECE,   // There's a piece been deleted
} GameFlags;

typedef struct
{
    // Cursor position
    int cursor_x;
    int cursor_y;

    // Locked piece position
    int locked_piece_x;
    int locked_piece_y;
    _Bool piece_locked; // there's a piece locked

    int board[8][8]; // Board map

    GameFlags flag; // Game events

    SDL_Event event; // SDL events
}GameContext;

void game();

#endif //GAME_H
