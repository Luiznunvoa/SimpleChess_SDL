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

#include "board.h"

#define DARK_CELL_COLOR 0x734B
#define BRIGHT_CELL_COLOR 0x9C90
#define BORDER_COLOR 0

#define BORDER_SIZE (board->rect.w / 100)
#define CELL_SIZE ((board->rect.w / 8) -1)

const char board_map[8][8] =
{
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
};

_Bool board_init(Element* board, SDL_Renderer** renderer)
{
    int pitch;
    void* pixels;
    SDL_PixelFormat* format;

    board->rect.w += 2 * BORDER_SIZE;
    board->rect.h += 2 * BORDER_SIZE;

    board->texture = SDL_CreateTexture(
        *renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        board->rect.w, board->rect.h
        );

    if (!board->texture)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Error creating texture: %s\n", SDL_GetError());
        return false;
    }

    if (!lock_texture_and_alloc_format(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565))
    {
        SDL_DestroyTexture(board->texture);
        return false;
    }

    Uint16* pixelData = (Uint16*)pixels;

    // Draws a border around the board(i'm just drawing a big black square than a smaller on top, calling the gap a border).
    draw_cell(pixelData, pitch, 0, 0, board->rect.w, BORDER_COLOR);

    // Iterates through board cells, filling each with one of two colors based on the board map.
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++)
        {
            const Uint16 color = (board_map[row][col] == '1') ?
            DARK_CELL_COLOR :
            BRIGHT_CELL_COLOR;

            const int start_x = (col * CELL_SIZE + BORDER_SIZE);
            const int start_y = (row * CELL_SIZE + BORDER_SIZE);

            draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);
        }

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    board->update = NULL;
    return true;
}

void draw_cell(
    Uint16* pixelData,
    const int pitch,
    const int startX, const int startY,
    const int size,
    const Uint16 color
    )
{
    for (int y = startY; y < startY + size; y++)
    {
        Uint16* row = pixelData + y * (pitch / 2) + startX;
        for (int x = 0; x < size; x++)
            row[x] = color;
    }
}

_Bool lock_texture_and_alloc_format(
    SDL_Texture* texture,
    void** pixels,
    int* pitch,
    SDL_PixelFormat** format,
    SDL_PixelFormatEnum enum_format
)
{
    if (SDL_LockTexture(texture, NULL, pixels, pitch) < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Error locking texture: %s\n", SDL_GetError());
        return false;
    }

    *format = SDL_AllocFormat(enum_format);
    if (!*format)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Error allocating pixel format: %s\n", SDL_GetError());
        SDL_UnlockTexture(texture);
        return false;
    }

    return true;
}