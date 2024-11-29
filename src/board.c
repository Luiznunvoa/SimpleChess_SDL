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

#define DARK_COLOR 0x734B   // Dark brown color
#define BRIGHT_COLOR 0x9C90 // Bright brown color
#define CURSOR_COLOR 0x2ce5 // Green
#define PIECE_COLOR 0x32f1  // Blue
#define LOCKED_COLOR 0x6191 // Purple

// Original cell color based on row and column indices
#define ORIGINAL_COLOR(row, col) ((((row + col) % 2) ? '1' : '0') == '1') ? DARK_COLOR : BRIGHT_COLOR

int prev_x, prev_y = 0; // Previous cursor position

// Initialize the board texture and render its initial state
_Bool init_board(Element* board, SDL_Renderer* renderer)
{
    // Create a texture for the board
    board->texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        board->rect.w, board->rect.h
    );
    if (!board->texture)
    {
        SDL_LogCritical(0, "Error creating texture: %s\n", SDL_GetError());
        return false;
    }

    int pitch;               // Number of bytes in each row of the texture
    Uint16* pixels;          // Pointer to the texture's pixel buffer, cast to a 16-bit type for manipulating RGB565
    SDL_PixelFormat* format; // Structure describing the pixel format of the texture

    if (!setup_texture(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565)) // Lock texture
    {
        SDL_DestroyTexture(board->texture);
        return false;
    }

    for (int y = 0; y < 8; y++) // Draw the initial chessboard with alternating colors
        for (int x = 0; x < 8; x++)
            draw_rectangle(pixels, pitch, (SDL_Rect){x, y, CELL_W, CELL_H}, ORIGINAL_COLOR(y, x));

    board->update = update_board; // Assign the update function to the board's update function pointer

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);
    return true;
}

// Update the board state based on game context (e.g., cursor position or piece movement)
int update_board(Element* board, GameContext* game)
{
    int pitch;               // Number of bytes in each row of the texture
    Uint16* pixels;          // Pointer to the texture's pixel buffer, cast to a 16-bit type for manipulating RGB565
    SDL_PixelFormat* format; // Structure describing the pixel format of the texture

    if (!setup_texture(board->texture, &pixels, &pitch, &format, SDL_PIXELFORMAT_RGB565)) // Lock texture
    {
        SDL_DestroyTexture(board->texture);
        return error;
    }

    update_selected(game->cursor_x, game->cursor_y, game->board, pixels, pitch); // Update the selected cell's position

    if(game->cursor_x != game->lock_x || game->cursor_y != game->lock_y) // Verify if the cursor is on the lock
        update_locked(game->lock_x, game->lock_y, game->piece_locked, pixels, pitch); // Update the locked cell

    SDL_FreeFormat(format);
    SDL_UnlockTexture(board->texture);
    return false;
}

// Draw the currently selected cell and update its appearance
void update_selected(const int x, const int y, int map[8][8], Uint16* pixels, const int pitch)
{
    if (prev_x != x || prev_y != y) // Check if the cursor position has changed to restore the original color
        draw_rectangle(pixels, pitch, (SDL_Rect){prev_x, prev_y, CELL_W, CELL_H}, ORIGINAL_COLOR(prev_y, prev_x));

    draw_rectangle(pixels, pitch, (SDL_Rect){x, y, CELL_W, CELL_H}, map[y][x] ? PIECE_COLOR : CURSOR_COLOR);

    // Update the last cursor position
    prev_x = x;
    prev_y = y;
}

// Draw the currently locked cell and update its appearance
void update_locked(const int x, const int y, const _Bool piece_locked, Uint16* pixels, const int pitch)
{
    if(piece_locked)
        draw_rectangle(pixels, pitch, (SDL_Rect){x, y, CELL_W, CELL_H}, LOCKED_COLOR);
    else
        draw_rectangle(pixels, pitch, (SDL_Rect){x, y, CELL_W, CELL_H}, ORIGINAL_COLOR(x, y));
}

// Lock a texture and allocate pixel format for rendering updates
_Bool setup_texture(SDL_Texture* texture, Uint16** pixels, int* pitch, SDL_PixelFormat** format, const Uint32 type)
{
    void* pixel_buffer; // Generic pointer to the texture's pixel buffer

    if (SDL_LockTexture(texture, NULL, &pixel_buffer, pitch) > 0)
    {
        SDL_LogCritical(0, "Error locking texture: %s\n", SDL_GetError());
        return false;
    }

    *format = SDL_AllocFormat(type);
    if (!*format)
    {
        SDL_LogCritical(0, "Error allocating pixel format: %s\n", SDL_GetError());
        SDL_UnlockTexture(texture);
        return false;
    }
    *pixels = pixel_buffer; // Cast the pixel buffer to a 16-bit pointer for color manipulation
    return true;
}

// Draw a square cell on the chessboard with the specified color
void draw_rectangle(Uint16* pixels, const int pitch, const SDL_Rect rect, const Uint16 color)
{
    // Calculate the starting position of the square in the texture
    const int start_x = rect.x * rect.w ;
    const int start_y = rect.y * rect.h;

    for (int i = start_y; i < start_y + rect.h; i++) // Fill the square with the specified color
    {
        Uint16* row = pixels + i * (pitch / 2) + start_x;
        for (int j = 0; j < rect.w; j++)
            row[j] = color;
    }
}