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

#include <stdio.h>

#include "board.h"

#define BORDER_SIZE (board->rect.w / 100)
#define BOARD_SIZE ((board->rect.w / 8) -1)
#define SELECTION_COLOR 2783
#define BORDER_COLOR 0
#define PIXEL_DATA
#define START_X (col * BOARD_SIZE + BORDER_SIZE)
#define START_Y (row * BOARD_SIZE + BORDER_SIZE)

const char board_map[8][8] = {
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
    {'0', '1', '0', '1', '0', '1', '0', '1'},
    {'1', '0', '1', '0', '1', '0', '1', '0'},
};

BoardData board_data;

bool board_init(Element* board, SDL_Renderer* renderer)
{
    void* pixels;
    int pitch;

    board->rect.w += 2 * BORDER_SIZE;
    board->rect.h += 2 * BORDER_SIZE;

    board->texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        board->rect.w, board->rect.h
        );

    if (!board->texture)
    {
        printf("Error creating texture: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_LockTexture(board->texture, NULL, &pixels, &pitch) < 0)
    {
        printf("Error locking texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(board->texture);
        return false;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB565);

    if (!format)
    {
        printf("Error allocating format: %s\n", SDL_GetError());
        SDL_UnlockTexture(board->texture);
        SDL_DestroyTexture(board->texture);
        return false;
    }

    Uint16* pixelData = (Uint16*)pixels;

    draw_border(pixelData, pitch, 0, 0, board->rect.w, BORDER_COLOR);

    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++)
        {
            const Uint16 color = (board_map[row][col] == '1') ?
            SDL_MapRGB(format, board->color1.r, board->color1.g, board->color1.b) :
            SDL_MapRGB(format, board->color2.r, board->color2.g, board->color2.b);

            draw_border(pixelData, pitch, START_X, START_Y, BOARD_SIZE, color);
        }

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    board_data = (BoardData){0};
    return true;
}

bool board_update(const Element* board)
{
    void* pixels;
    int pitch;

    if (SDL_LockTexture(board->texture, NULL, &pixels, &pitch) < 0)
    {
        printf("Error locking texture:: %s\n", SDL_GetError());
        SDL_DestroyTexture(board->texture);
        return false;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB565);

    if (!format)
    {
        printf("Error allocating format: %s\n", SDL_GetError());
        SDL_UnlockTexture(board->texture);
        return false;
    }

    Uint16* pixelData = (Uint16*)pixels;

    draw_selected_cell(board, BORDER_SIZE, BOARD_SIZE, format, pixelData, pitch, SELECTION_COLOR);

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    return true;
}

void draw_selected_cell(
    const Element* board,
    const int border_size,
    const int board_size,
    const SDL_PixelFormat* format,
    Uint16* pixelData,
    const int pitch,
    const Uint16 selection_color
    )
{
    if (board_data.previous_select_x != -1 && board_data.previous_select_y != -1)
    {
        const int prev_start_x = border_size + board_data.previous_select_x * board_size;
        const int prev_start_y = border_size + board_data.previous_select_y * board_size;

        const Uint16 originalColor = (board_map[board_data.previous_select_y][board_data.previous_select_x] == '1') ?
            SDL_MapRGB(format, board->color1.r, board->color1.g, board->color1.b) :
            SDL_MapRGB(format, board->color2.r, board->color2.g, board->color2.b);

        draw_border(pixelData, pitch, prev_start_x, prev_start_y, board_size, originalColor);
    }

    const int start_x = border_size + board_data.select_x * board_size;
    const int start_y = border_size + board_data.select_y * board_size;

    draw_border(pixelData, pitch, start_x, start_y, board_size, selection_color);

    board_data.previous_select_x = board_data.select_x;
    board_data.previous_select_y = board_data.select_y;

}
void draw_border(
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