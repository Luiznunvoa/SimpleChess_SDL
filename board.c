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
#include "game.h"
#include "pieces.h"

#define SELECTION_COLOR 2783
#define PIECE_COLOR 903
#define BORDER_COLOR 0

#define BORDER_SIZE (board->rect.w / 100)
#define CELL_SIZE ((board->rect.w / 8) -1)

// The board cells color pattern
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

BoardData board_data; // Structure with the board selection cursor information
bool update_state; // Variable defining if there is a need to update the screen

// Initializes the board with a texture and fills it with colors based on a board map.
bool board_init(Element* board, SDL_Renderer* renderer)
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
        printf("Error creating texture: %s\n", SDL_GetError());
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
            SDL_MapRGB(format, board->color1.r, board->color1.g, board->color1.b) :
            SDL_MapRGB(format, board->color2.r, board->color2.g, board->color2.b);

            const int start_x = (col * CELL_SIZE + BORDER_SIZE);
            const int start_y = (row * CELL_SIZE + BORDER_SIZE);

            draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);
        }

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    board_data = (BoardData){0}; // Initializes board selection data structure.
    return true;
}

// Updates the board, redrawing selected and locked pieces if needed.
bool board_update(Element* board)
{
    int pitch;
    void* pixels;
    SDL_PixelFormat* format;

    if (!lock_texture_and_alloc_format(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565))
    {
        SDL_DestroyTexture(board->texture);
        return false;
    }

    Uint16* pixelData = (Uint16*)pixels;

    draw_selected_cell(board, format, pixelData, pitch);
    draw_locked_piece(board, format, pixelData, pitch);

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);
    update = update_state;

    return true;
}

// Draws the cell that is currently selected on the board.
void draw_selected_cell(
    const Element* board,
    const SDL_PixelFormat* format,
    Uint16* pixelData,
    const int pitch
    )
{
    // Reverts color of previously selected cell, if any.
    if (board_data.previous_select_x != -1 && board_data.previous_select_y != -1)
    {
        const Uint16 originalColor = (board_map[board_data.previous_select_y][board_data.previous_select_x] == '1') ?
            SDL_MapRGB(format, board->color1.r, board->color1.g, board->color1.b) :
            SDL_MapRGB(format, board->color2.r, board->color2.g, board->color2.b);

        const int start_x = (BORDER_SIZE + board_data.previous_select_x * CELL_SIZE);
        const int start_y =  (BORDER_SIZE + board_data.previous_select_y * CELL_SIZE);

        draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, originalColor);
    }

    Uint16 color = SELECTION_COLOR;

    // Updates color if selected cell matches locked piece position.
    if(board_data.previous_select_x == selected_piece.x && board_data.previous_select_y == selected_piece.y)
        color = PIECE_COLOR;

    const int start_x = (BORDER_SIZE + board_data.select_x * CELL_SIZE);
    const int start_y = (BORDER_SIZE + board_data.select_y * CELL_SIZE);

    draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);

    // Updates board state the select cursor changed position
    if(board_data.previous_select_x == board_data.select_x && board_data.previous_select_y == board_data.select_y)
        update_state = false;
    else
        update_state = true;

    board_data.previous_select_x = board_data.select_x;
    board_data.previous_select_y = board_data.select_y;
}

// Draws the locked piece on the board.
void draw_locked_piece(
    const Element* board,
    const SDL_PixelFormat* format,
    Uint16* pixelData,
    int pitch
)
{
    const int start_x = BORDER_SIZE + selected_piece.x * CELL_SIZE;
    const int start_y = BORDER_SIZE + selected_piece.y * CELL_SIZE;

    bool is_adjacent = (abs(selected_piece.x - board_data.select_x) <= 1) &&
                       (abs(selected_piece.y - board_data.select_y) <= 1);

    // Draws locked piece if adjacent to selected cell(is needed because the color would be overwritten by the other function).
    // probably there is a smarter way of doing it though.
    if (selected_piece.locked && is_adjacent)
    {
        selected_piece.previous_x = selected_piece.x;
        selected_piece.previous_y = selected_piece.y;

        draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, PIECE_COLOR);
    }
    // Removes locked piece.
    else if (!selected_piece.locked &&
             (board_data.select_x != selected_piece.x || board_data.select_y != selected_piece.y))
    {
        Uint16 color = (board_map[selected_piece.previous_x][selected_piece.previous_y] == '1') ?
            SDL_MapRGB(format, board->color1.r, board->color1.g, board->color1.b) :
            SDL_MapRGB(format, board->color2.r, board->color2.g, board->color2.b);

        const int prev_x = BORDER_SIZE + selected_piece.previous_x * CELL_SIZE;
        const int prev_y = BORDER_SIZE + selected_piece.previous_y * CELL_SIZE;

        selected_piece.previous_x = selected_piece.x;
        selected_piece.previous_y = selected_piece.y;

        draw_cell(pixelData, pitch, prev_x, prev_y, CELL_SIZE, color);
    }
}

// Locks texture and allocates pixel format for safe pixel access.
bool lock_texture_and_alloc_format(
    SDL_Texture* texture,
    void** pixels,
    int* pitch,
    SDL_PixelFormat** format,
    SDL_PixelFormatEnum enum_format
)
{
    if (SDL_LockTexture(texture, NULL, pixels, pitch) < 0)
    {
        printf("Error locking texture: %s\n", SDL_GetError());
        return false;
    }

    *format = SDL_AllocFormat(enum_format);
    if (!*format)
    {
        printf("Error allocating pixel format: %s\n", SDL_GetError());
        SDL_UnlockTexture(texture);
        return false;
    }

    return true;
}

// Draws a cell with a given color at specified position and size.
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