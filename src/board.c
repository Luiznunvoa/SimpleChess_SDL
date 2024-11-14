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
#define CURSOR_CELL_COLOR 0x2ce5
#define PIECE_CELL_COLOR 0x32f1
#define ORIGINAL_CELL_COLOR(row, col) ((((row + col) % 2) ? '1' : '0') == '1') ? DARK_CELL_COLOR : BRIGHT_CELL_COLOR;

#define BORDER_SIZE (board->rect.w / 100)
#define CELL_SIZE ((board->rect.w / 8) -1)

BoardData data = (BoardData){0};

_Bool init_board(Element* board, SDL_Renderer* renderer)
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

    board->update = update_board;
    return true;
}

int update_board(Element const* board, GameContext* game)
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

    draw_selected_cell(
        board,
        game->cursor_x, game->cursor_y,
        game->board,
        pixelData,
        pitch
        );

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    return false;
}

void draw_selected_cell(
    const Element* board,
    const int cursor_x, const int cursor_y,
    int board_map[8][8],
    Uint16* pixelData,
    const int pitch
)
{
    if (data.last_cursor_pos_x != -1 && data.last_cursor_pos_y != -1)
    {
        const Uint16 originalColor = ORIGINAL_CELL_COLOR(data.last_cursor_pos_y, data.last_cursor_pos_x);

        const int start_x = (BORDER_SIZE + data.last_cursor_pos_x * CELL_SIZE);
        const int start_y = (BORDER_SIZE + data.last_cursor_pos_y * CELL_SIZE);

        draw_square(pixelData, pitch, start_x, start_y, CELL_SIZE, originalColor);
    }

    Uint16 color;

    if(board_map[cursor_y][cursor_x] == 0)
        color = CURSOR_CELL_COLOR;
    else
        color = PIECE_CELL_COLOR;

    const int start_x = (BORDER_SIZE + cursor_x * CELL_SIZE);
    const int start_y = (BORDER_SIZE + cursor_y * CELL_SIZE);

    draw_square(pixelData, pitch, start_x, start_y, CELL_SIZE, color);

    data.last_cursor_pos_x = cursor_x;
    data.last_cursor_pos_y = cursor_y;
}

void draw_locked_cell(
    const Element* board,
    const int locked_piece_x, const int locked_piece_y,
    const _Bool locked,
    Uint16* pixelData,
    const int pitch
)
{
    const int start_x = (BORDER_SIZE + locked_piece_x * CELL_SIZE);
    const int start_y = (BORDER_SIZE + locked_piece_y * CELL_SIZE);

    draw_square(pixelData, pitch, start_x, start_y, CELL_SIZE, PIECE_CELL_COLOR);
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
    if (SDL_LockTexture(texture, NULL, pixels, pitch) > 0)
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
