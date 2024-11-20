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

#define DARK_COLOR 0x734B      // Dark brown color
#define BRIGHT_COLOR 0x9C90    // Bright brown color
#define CURSOR_COLOR 0x2ce5    // Green
#define PIECE_COLOR 0x32f1     // Blue

// Macro to determine the original cell color based on row and column indices
#define ORIGINAL_COLOR(row, col) ((((row + col) % 2) ? '1' : '0') == '1') ? DARK_COLOR : BRIGHT_COLOR

int prev_cursor_x, prev_cursor_y = 0;

// Initialize the board texture and render its initial state
_Bool init_board(Element* board, SDL_Renderer* renderer)
{
    int pitch;
    void* pixels;
    SDL_PixelFormat* format;

    // Create a texture for the board
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

    // Cast the pixel buffer to a 16-bit pointer for color manipulation
    Uint16* pixelData = (Uint16*)pixels;

    // Draw the initial chessboard with alternating colors
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            draw_rectangle(pixelData, pitch, x, y, board->rect.w, board->rect.h, ORIGINAL_COLOR(y, x));

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);

    board->update = update_board; // Assign the update function to the board
    return true;
}

// Update the board state based on game context (e.g., cursor position or piece movement)
int update_board(Element const* board, GameContext* game)
{
    int pitch;
    void* pixels;
    SDL_PixelFormat* format;

    // Lock the texture for updates
    if (!lock_texture_and_alloc_format(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565))
    {
        SDL_DestroyTexture(board->texture);
        return error;
    }

    // Cast the pixel buffer to a 16-bit pointer for color manipulation
    Uint16* pixelData = (Uint16*)pixels;

    // Update the selected cell's position
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

// Draw the currently selected cell and update its appearance
void draw_selected_cell(
    const Element* board,
    const int cursor_x, const int cursor_y,
    int board_map[8][8],
    Uint16* pixelData,
    const int pitch
)
{
    // Check if the cursor position has changed
    if (prev_cursor_x != cursor_x || prev_cursor_y != cursor_y)
        // Restore the original color of the previous cursor cell
        draw_rectangle(
            pixelData,
            pitch,
            prev_cursor_x, prev_cursor_y,
            board->rect.w, board->rect.h,
            ORIGINAL_COLOR(prev_cursor_y, prev_cursor_x)
        );

    Uint16 color;

    if (board_map[cursor_y][cursor_x] == 0) // No piece on the cell
        color = CURSOR_COLOR;
    else // A piece is present on the cell
        color = PIECE_COLOR;

    draw_rectangle(pixelData, pitch, cursor_x, cursor_y, board->rect.w, board->rect.h, color);

    // Update the last cursor position
    prev_cursor_x = cursor_x;
    prev_cursor_y = cursor_y;
}

// Lock a texture and allocate pixel format for rendering updates
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

// Draw a square cell on the chessboard with the specified color
void draw_rectangle(
    Uint16* pixelData,
    const int pitch,
    const int x, const int y,
    const int w, const int h,
    const Uint16 color
)
{
    // Calculate the starting position of the square in the texture
    const int start_x = (x * (w / 8));
    const int start_y = (y * (h / 8));

    // Fill the square with the specified color
    for (int i = start_y; i < start_y + (h / 8); i++)
    {
        Uint16* row = pixelData + i * (pitch / 2) + start_x;
        for (int j = 0; j < (w / 8); j++)
            row[j] = color;
    }
}
