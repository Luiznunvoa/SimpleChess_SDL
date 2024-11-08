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
#include "pieces.h"
#include "game.h"

#define DARK_CELL_COLOR 0x734B
#define BRIGHT_CELL_COLOR 0x9C90
#define CURSOR_CELL_COLOR 0x2ce5
#define PIECE_CELL_COLOR 0x32f1
#define ORIGINAL_CELL_COLOR(row, col) ((((row + col) % 2) ? '1' : '0') == '1') ? DARK_CELL_COLOR : BRIGHT_CELL_COLOR;
#define BORDER_COLOR 0x0000

#define BORDER_SIZE (board->rect.w / 100)
#define CELL_SIZE ((board->rect.w / 8) -1)

BoardData board_data = (BoardData){0};

_Bool board_init(Element* board, SDL_Renderer* renderer)
{
    int pitch;
    void* pixels;
    SDL_PixelFormat* format;

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
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error creating texture: %s\n", SDL_GetError());
        return false;
    }

    if (!lock_texture_and_alloc_format(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565))
    {
        SDL_DestroyTexture(board->texture);
        return false;
    }

    Uint16* pixelData = (Uint16*)pixels;


    draw_square(pixelData, pitch, 0, 0, board->rect.w, BORDER_COLOR);

    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++)
        {
            const Uint16 color = ORIGINAL_CELL_COLOR(row, col);

            const int start_x = (col * CELL_SIZE + BORDER_SIZE);
            const int start_y = (row * CELL_SIZE + BORDER_SIZE);

            draw_square(pixelData, pitch, start_x, start_y, CELL_SIZE, color);
        }

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    board->update = board_update;
    return true;
}

int board_update(const Element* board, GameContext* game)
{
    int pitch;
    void* pixels;
    SDL_PixelFormat* format;

    if (!lock_texture_and_alloc_format(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565))
    {
        SDL_DestroyTexture(board->texture);
        return error;
    }

    Uint16* pixelData = (Uint16*)pixels;

    const _Bool result = false;

    draw_selected_cell(board, game->cursor_x, game->cursor_y, &game->selected, &game->selecting, format, pixelData, pitch);

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    return result;
}

void draw_selected_cell(
    const Element* board,
    int cursor_x, int cursor_y,
    _Bool* selected, _Bool* selecting,
    const SDL_PixelFormat* format,
    Uint16* pixelData,
    const int pitch
)
{
    if (board_data.last_cursor_x != -1 && board_data.last_cursor_y != -1)
    {
        const Uint16 originalColor = ORIGINAL_CELL_COLOR(board_data.last_cursor_y, board_data.last_cursor_x);

        const int start_x = (BORDER_SIZE + board_data.last_cursor_x * CELL_SIZE);
        const int start_y = (BORDER_SIZE + board_data.last_cursor_y * CELL_SIZE);

        draw_square(pixelData, pitch, start_x, start_y, CELL_SIZE, originalColor);
    }

    Uint16 color;

    if(piece_board[cursor_y][cursor_x] == 0)
    {
        color = CURSOR_CELL_COLOR;
        *selecting = false;
        *selected = true;
    }
    else
    {
        color = PIECE_CELL_COLOR;
        *selected = false;
    }

    const int start_x = (BORDER_SIZE + cursor_x * CELL_SIZE);
    const int start_y = (BORDER_SIZE + cursor_y * CELL_SIZE);

    draw_square(pixelData, pitch, start_x, start_y, CELL_SIZE, color);

    board_data.last_cursor_x = cursor_x;
    board_data.last_cursor_y = cursor_y;
}

void draw_square(
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
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error locking texture: %s\n", SDL_GetError());
        return false;
    }

    *format = SDL_AllocFormat(enum_format);
    if (!*format)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error allocating pixel format: %s\n", SDL_GetError());
        SDL_UnlockTexture(texture);
        return false;
    }

    return true;
}
