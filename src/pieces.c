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

#include "pieces.h"

_Bool init_pieces(Element* piece, SDL_Renderer* renderer)
{
    typedef struct {
        Uint8 piece_number;
        const char* bmp_path;
    } PieceMap;

    const PieceMap piece_map[] = {
        {1, "assets/black_rook.bmp"},
        {2, "assets/black_horse.bmp"},
        {3, "assets/black_bishop.bmp"},
        {4, "assets/black_queen.bmp"},
        {5, "assets/black_king.bmp"},
        {6, "assets/black_bishop.bmp"},
        {7, "assets/black_horse.bmp"},
        {8, "assets/black_rook.bmp"},
        {9, "assets/black_pawn.bmp"},
        {10, "assets/black_pawn.bmp"},
        {11, "assets/black_pawn.bmp"},
        {12, "assets/black_pawn.bmp"},
        {13, "assets/black_pawn.bmp"},
        {14, "assets/black_pawn.bmp"},
        {15, "assets/black_pawn.bmp"},
        {16, "assets/black_pawn.bmp"},
        {17, "assets/white_pawn.bmp"},
        {18, "assets/white_pawn.bmp"},
        {19, "assets/white_pawn.bmp"},
        {20, "assets/white_pawn.bmp"},
        {21, "assets/white_pawn.bmp"},
        {22, "assets/white_pawn.bmp"},
        {23, "assets/white_pawn.bmp"},
        {24, "assets/white_pawn.bmp"},
        {25, "assets/white_rook.bmp"},
        {26, "assets/white_horse.bmp"},
        {27, "assets/white_bishop.bmp"},
        {28, "assets/white_queen.bmp"},
        {29, "assets/white_king.bmp"},
        {30, "assets/white_bishop.bmp"},
        {31, "assets/white_horse.bmp"},
        {32, "assets/white_rook.bmp"},
    };

    const char* bmp_path = NULL;    // File path to the bitmap image for the element

    for (int i = 0; i < 32; i++)
        if (piece->info == piece_map[i].piece_number)
        {
            bmp_path = piece_map[i].bmp_path;
            break;
        }

    SDL_Surface* pieces_surface = SDL_LoadBMP(bmp_path);
    if (!pieces_surface)
    {
        SDL_LogCritical(0, "Error loading the BMP file to the surface: %s\n", SDL_GetError());
        return false;
    }

    piece->texture = SDL_CreateTextureFromSurface(renderer, pieces_surface);

    piece->rect.w = pieces_surface->w;
    piece->rect.h = pieces_surface->h;

    SDL_FreeSurface(pieces_surface);

    if (!piece->texture)
    {
        SDL_LogCritical(0, "Error creating the texture: %s\n", SDL_GetError());
        return false;
    }

    piece->update = update_pieces;
    return true;
}

int update_pieces(Element* piece, GameContext* game)
{
    const int x = (piece->rect.x - OFFSET_X) / CELL_W;
    const int y = (piece->rect.y - OFFSET_Y) / CELL_H;

    // Movimento da peça
    if (game->flag == MOVE_PIECE && 
        !game->board[game->cursor_y][game->cursor_x] &&
        x == game->lock_x && y == game->lock_y)
    {
        game->flag = NOTHING;
        game->board[y][x] = 0;
        piece->rect.x = (game->cursor_x * CELL_W) + OFFSET_X;
        piece->rect.y = (game->cursor_y * CELL_H) + OFFSET_Y;
        game->piece_locked = false;
        return true;
    }

    // Verificação de cursor
    if (game->cursor_x != x || game->cursor_y != y)
        return false;

    // Ações baseadas na flag
    switch (game->flag)
    {
        case SELECT_CELL:
            game->flag = NOTHING;
            return true;

        case LOCK_PIECE:
            game->flag = NOTHING;
            game->piece_locked = true;
            game->lock_x = x;
            game->lock_y = y;
            return true;

        case DELETE_PIECE:
            return 2;

        default:
            return false;
    }
}