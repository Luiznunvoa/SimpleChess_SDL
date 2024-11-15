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

#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "ui.h"

typedef struct
{
    int last_cursor_pos_x;
    int last_cursor_pos_y;
}BoardData;

_Bool init_board(Element* board, SDL_Renderer* renderer);
int update_board(const Element* board, GameContext* game);
void draw_selected_cell(
    const Element* board,
    const int cursor_x, const int cursor_y,
    int board_map[8][8],
    Uint16* pixelData,
    int pitch
);
_Bool lock_texture_and_alloc_format(
    SDL_Texture* texture,
    void** pixels,
    int* pitch,
    SDL_PixelFormat** format,
    SDL_PixelFormatEnum enum_format
);
void draw_square(
    Uint16* pixelData,
    const int pitch,
    const int pos_x, const int pos_y,
    const int width, const int height,
    const Uint16 color
);

#endif //BOARD_H
