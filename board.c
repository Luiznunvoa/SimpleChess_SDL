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

bool board_init(Element* board, SDL_Renderer* renderer)
{
    void* pixels;
    int pitch;

    const int border_size = (board->rect.w / 100);
    const int grid_size = board->rect.w / 8;

    board->rect.w = board->rect.w + (2 * border_size);
    board->rect.h = board->rect.h + (2 * border_size);

    board->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, board->rect.w, board->rect.h);
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
    const Uint16 borderColor = SDL_MapRGB(format, 0, 0, 0);
    const Uint16 gridColor1 = SDL_MapRGB(format, board->color1.r, board->color1.g, board->color1.b);
    const Uint16 gridColor2 = SDL_MapRGB(format, board->color2.r, board->color2.g, board->color2.b);

    for (int y = 0; y < board->rect.h; y++)
        for (int x = 0; x < board->rect.w; x++)
            if (x < border_size || x >= board->rect.w - border_size || y < border_size || y >= board->rect.h - border_size)
                pixelData[y * (pitch / 2) + x] = borderColor;

    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++)
        {
            const Uint16 color = (board_map[row][col] == '1') ? gridColor1 : gridColor2;
            const int startY = row * grid_size + border_size;
            const int endY = (row + 1) * grid_size + border_size;
            const int startX = col * grid_size + border_size;
            const int endX = (col + 1) * grid_size + border_size;

            for (int y = startY; y < endY; y++)
                for (int x = startX; x < endX; x++)
                    pixelData[y * (pitch / 2) + x] = color;
        }

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    printf("Board Loaded\n");
    return true;
}