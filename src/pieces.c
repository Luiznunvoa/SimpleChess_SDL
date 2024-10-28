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

#include "pieces.h"
#include "ui.h"
#include "board.h"
#include "game.h"

// Defines a mapping between bitmap paths and piece types.
typedef struct {
    const char* bmp_path; // Path to the bitmap file for a piece.
    int piece;            // Integer identifier for the piece type.
    ELM_update update;
} PieceMap;

// Maps each chess piece type to its corresponding bitmap file.
PieceMap piece_map[] = {
    {"../../assets/black_pawn.bmp", BLACK_PAWN, pawn_update},
    {"../../assets/white_pawn.bmp", WHITE_PAWN, pawn_update},
    {"../../assets/black_horse.bmp", BLACK_HORSE, pieces_update},
    {"../../assets/white_horse.bmp", WHITE_HORSE, pieces_update},
    {"../../assets/black_bishop.bmp", BLACK_BISHOP, pieces_update},
    {"../../assets/white_bishop.bmp", WHITE_BISHOP, pieces_update},
    {"../../assets/black_rook.bmp", BLACK_ROOK, pieces_update},
    {"../../assets/white_rook.bmp", WHITE_ROOK, pieces_update},
    {"../../assets/black_queen.bmp", BLACK_QUEEN, pieces_update},
    {"../../assets/white_queen.bmp", WHITE_QUEEN, pieces_update},
    {"../../assets/black_king.bmp", BLACK_KING, pieces_update},
    {"../../assets/white_king.bmp", WHITE_KING, pieces_update}
};

// Initializes the chessboard with piece positions.
Uint8 piece_board[8][8] = {
    {7, 3, 5, 9, 11, 5, 3, 7},   // Row with black pieces.
    {1, 1, 1, 1, 1, 1, 1, 1},     // Row with black pawns.
    {0, 0, 0, 0, 0, 0, 0, 0},     // Empty row.
    {0, 0, 0, 0, 0, 0, 0, 0},     // Empty row.
    {0, 0, 0, 0, 0, 0, 0, 0},     // Empty row.
    {0, 0, 0, 0, 0, 0, 0, 0},     // Empty row.
    {2, 2, 2, 2, 2, 2, 2, 2},     // Row with white pawns.
    {8, 4, 6, 10, 12, 6, 4, 8}    // Row with white pieces.
};

SelectedPiece selected_piece;// Struct for storing selected piece information.

// Initializes a piece, setting its texture and bitmap path.
bool pieces_init(Element* piece, SDL_Renderer* renderer)
{
    // Finds the bitmap path for the piece type and the update function.
    for(int i = 0; i < 12; i++)
        if(piece_map[i].piece == piece->type)
        {
            piece->bmp_path = piece_map[i].bmp_path;
            piece->update = piece_map[i].update;
            break;
        }

    // Loads the bitmap image file into an SDL surface.
    SDL_Surface* pieces_surface = SDL_LoadBMP(piece->bmp_path);
    if (!pieces_surface)
    {
        printf("Error loading the BMP file to the surface: %s\n", SDL_GetError());
        return false;
    }

    // Creates a texture from the loaded surface.
    piece->texture = SDL_CreateTextureFromSurface(renderer, pieces_surface);

    // Sets the width and height of the piece's rectangle to match the surface.
    piece->rect.w = pieces_surface->w;
    piece->rect.h = pieces_surface->h;

    SDL_FreeSurface(pieces_surface);

    if (!piece->texture)
    {
        printf("Error creating the texture: %s\n", SDL_GetError());
        return false;
    }

    selected_piece.locked = false;
    selected_piece.lock = false;
    return true;
}

// Generic piece update function for debug purposes
bool pieces_update(Element* piece)
{
    selected_piece.locked = selected_piece.lock;
    // Calculates the piece's position on the board based on screen coordinates.
    const int x = (piece->rect.x - 45) / 65;
    const int y = (piece->rect.y - 45) / 65;

    // Checks if the current piece matches the selected board position and isn't locked.
    if(board_data.select_x == x && board_data.select_y == y && !selected_piece.lock)
    {
        selected_piece.x = x;
        selected_piece.y = y;
    }
    return true;
}

// Updates the pawns verifying if it's possible moves and saving in selected_piece.possible_moves(still buggy)
bool pawn_update(Element* piece)
{
    const int x = (piece->rect.x - 45) / 65;
    const int y = (piece->rect.y - 45) / 65;

    selected_piece.locked = selected_piece.lock;

    if(x < 0 || y < 0)
        return false;

    if (board_data.select_x == x && board_data.select_y == y && !selected_piece.lock)
    {
        selected_piece.x = x;
        selected_piece.y = y;
        return true;
    }
    if (board_data.select_x == x && board_data.select_y == y && selected_piece.lock && selected_piece.x == x && selected_piece.y == y)
    {
        memset(selected_piece.possible_moves, -1, sizeof(selected_piece.possible_moves));

        const int direction = (piece->type == WHITE_PAWN) ? -1 : 1;

        selected_piece.possible_moves[0] = (Move){.x = x, .y = y + direction};

        update = true;

        return true;
    }
    return true;
}