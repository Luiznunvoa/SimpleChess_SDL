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
    NOTHING,        // Nothing is happening
    QUIT_GAME,      // Exit the game
    SELECT_CELL,    // There's a cell been selected
    LOCK_PIECE,     // There's a piece been locked
    DELETE_PIECE,   // There's a piece been deleted
} GameFlags;

typedef struct
{
    int cursor_x, cursor_y; // Cursor position

    int locked_x, locked_y; // Locked piece position

    _Bool piece_locked;     // If there's a piece locked

    int board[8][8];        // Board map

    GameFlags flag;         // Game events

    SDL_Event event;        // SDL events

    SDL_Renderer* renderer; // Game renderer handle
    SDL_Window* window;     // Game window handle
}GameContext;

void game();
_Bool init(SDL_Renderer** renderer, SDL_Window** window);
void quit(SDL_Renderer** renderer, SDL_Window** window);

#endif //GAME_H
