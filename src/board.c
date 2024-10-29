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

#define SELECTION_COLOR 0x2ce5
#define PIECE_COLOR 0x32f1
#define LOCKED_PIECE_COLOR 0x79b2
#define DARK_CELL_COLOR 0x734B
#define BRIGHT_CELL_COLOR 0x9C90
#define MOVE_SELECTION_COLOR 0x16e2
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

int last_selected_x; // x value of the last piece selected
int last_selected_y; // y value of the last piece selected

int last_locked_x; // x value of the last piece locked
int last_locked_y; // y value of the last piece locked

// Initializes the board with a texture and fills it with colors based on a board map.
int board_init(Element* board, SDL_Renderer* renderer)
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
            DARK_CELL_COLOR :
            BRIGHT_CELL_COLOR;

            const int start_x = (col * CELL_SIZE + BORDER_SIZE);
            const int start_y = (row * CELL_SIZE + BORDER_SIZE);

            draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);
        }

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    board_data = (BoardData){0}; // Initializes board selection data structure.
    board->update = board_update;
    return true;
}

// Updates the board, redrawing selected and locked pieces if needed.
int board_update(Element* board)
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
    highlight_possible_moves(board, pixelData, pitch, format);

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

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
    if (last_selected_x != -1 &&last_selected_y != -1)
    {
        const Uint16 originalColor = (board_map[last_selected_y][last_selected_x] == '1') ?
            DARK_CELL_COLOR :
            BRIGHT_CELL_COLOR;

        const int start_x = (BORDER_SIZE +last_selected_x * CELL_SIZE);
        const int start_y =  (BORDER_SIZE +last_selected_y * CELL_SIZE);

        draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, originalColor);
    }

    // If the last color was a move highlight it gets restored
    if(selected_piece.locked)
    {
        for (int i = 0; i < MAX_MOVES; i++)
        {
            const int move_x = selected_piece.possible_moves[i].x;
            const int move_y = selected_piece.possible_moves[i].y;

            if (move_x == last_selected_x && move_y == last_selected_y)
            {
                const int start_x = (move_x * CELL_SIZE) + BORDER_SIZE;
                const int start_y = (move_y * CELL_SIZE) + BORDER_SIZE;

                Uint16 color = SDL_MapRGB(format, 0, 255, 0);

                draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);
            }
        }
    }

    Uint16 color;

    // Updates color if selected cell matches locked piece position.
    if(last_selected_x == selected_piece.x &&last_selected_y == selected_piece.y)
        color = PIECE_COLOR;
    else
        color = SELECTION_COLOR;


    const int start_x = (BORDER_SIZE + board_data.select_x * CELL_SIZE);
    const int start_y = (BORDER_SIZE + board_data.select_y * CELL_SIZE);

    draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);

    // Updates board state the select cursor changed position
    if(last_selected_x == board_data.select_x &&last_selected_y == board_data.select_y)
        update = false;
    else
        update = true;

   last_selected_x = board_data.select_x;
   last_selected_y = board_data.select_y;
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
        last_locked_x = selected_piece.x;
        last_locked_y = selected_piece.y;

        draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, LOCKED_PIECE_COLOR);
    }
    // Removes locked piece.
    else if (!selected_piece.locked &&
             (board_data.select_x != selected_piece.x || board_data.select_y != selected_piece.y))
    {
        const Uint16 color = (board_map[last_locked_x][last_locked_y] == '1') ?
            DARK_CELL_COLOR :
            BRIGHT_CELL_COLOR;

        const int prev_x = BORDER_SIZE + last_locked_x * CELL_SIZE;
        const int prev_y = BORDER_SIZE + last_locked_y * CELL_SIZE;

        last_locked_x = selected_piece.x;
        last_locked_y = selected_piece.y;

        draw_cell(pixelData, pitch, prev_x, prev_y, CELL_SIZE, color);
    }
}

// Locks texture and allocates pixel format for safe pixel access.
int lock_texture_and_alloc_format(
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

// Highlights the possible moves of a piece based on selected_piece.possible_moves(TODO: fix this)
void highlight_possible_moves(const Element* board, Uint16* pixelData, const int pitch, const SDL_PixelFormat* format)
{
    // Verify if the piece selected is a pawn
    if (piece_board[board_data.select_y][board_data.select_x] != BLACK_PAWN && piece_board[board_data.select_y][board_data.select_x] != WHITE_PAWN)
        return;

    for (int i = 0; i < MAX_MOVES; i++)
    {
        const int move_x = selected_piece.possible_moves[i].x;
        const int move_y = selected_piece.possible_moves[i].y;

        // Secures if the movement coordinates are valid(useless yet)
        if (move_x >= 0 && move_x < 8 && move_y >= 0 && move_y < 8)
        {
            const int start_x = (move_x * CELL_SIZE) + BORDER_SIZE;
            const int start_y = (move_y * CELL_SIZE) + BORDER_SIZE;

            Uint16 color;

            // Verifies if the piece is locked, drawing or erasing the move highlights.
            if (selected_piece.locked)
            {
                color = MOVE_SELECTION_COLOR;
            }
            else
            {
                color = (board_map[move_y][move_x] == '1') ?
                    DARK_CELL_COLOR :
                    BRIGHT_CELL_COLOR;
            }

            draw_cell(pixelData, pitch, start_x, start_y, CELL_SIZE, color);
        }
    }
}
