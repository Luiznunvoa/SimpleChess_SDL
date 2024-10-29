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
    int select_x; // x value of the piece selected by the cursor
    int select_y; // y value of the piece selected by the cursor
}BoardData;

int board_init(Element* board, SDL_Renderer* renderer);
int board_update(Element* board);
void draw_selected_cell(
    const Element* board,
    const SDL_PixelFormat* format,
    Uint16* pixelData,
    int pitch
    );
void draw_locked_piece(
    const Element* board,
    const SDL_PixelFormat* format,
    Uint16* pixelData,
    int pitch
    );
int lock_texture_and_alloc_format(
    SDL_Texture* texture,
    void** pixels,
    int* pitch,
    SDL_PixelFormat** format,
    SDL_PixelFormatEnum enum_format
);
void draw_cell(
    Uint16* pixelData,
    int pitch,
    int startX, int startY,
    int size,
     Uint16 color
    );
void highlight_possible_moves(const Element* board, Uint16* pixelData, const int pitch, const SDL_PixelFormat* format);
extern BoardData board_data;

#endif //BOARD_H
