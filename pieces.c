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

typedef struct {
    const char* bmp_path;
    int piece;
} PieceMap;

PieceMap piece_map[] = {
    {"../../assets/black_pawn.bmp", BLACK_PAWN},
    {"../../assets/white_pawn.bmp", WHITE_PAWN},
    {"../../assets/black_horse.bmp", BLACK_HORSE},
    {"../../assets/white_horse.bmp", WHITE_HORSE},
    {"../../assets/black_bishop.bmp", BLACK_BISHOP},
    {"../../assets/white_bishop.bmp", WHITE_BISHOP},
    {"../../assets/black_rook.bmp", BLACK_ROOK},
    {"../../assets/white_rook.bmp", WHITE_ROOK},
    {"../../assets/black_queen.bmp", BLACK_QUEEN},
    {"../../assets/white_queen.bmp", WHITE_QUEEN},
    {"../../assets/black_king.bmp", BLACK_KING},
    {"../../assets/white_king.bmp", WHITE_KING}
};

bool pieces_init(Element* piece, SDL_Renderer* renderer)
{

    for(int i = 0; i < 12; i++)
        if(piece_map[i].piece == piece->type)
        {
            piece->bmp_path = piece_map[i].bmp_path;
            break;
        }

    SDL_Surface* pieces_surface = SDL_LoadBMP(piece->bmp_path);

    if (!pieces_surface)
    {
        printf("Error loading the BMP file to the surface: %s\n", SDL_GetError());
        return false;
    }

    piece->texture = SDL_CreateTextureFromSurface(renderer, pieces_surface);

    piece->rect.w = pieces_surface->w;
    piece->rect.h = pieces_surface->h;

    SDL_FreeSurface(pieces_surface);

    if (!piece->texture)
    {
        printf("Error creating the texture: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool pieces_update(Element* piece)
{
    const int x = (piece->rect.x - 45) / 65;
    const int y = (piece->rect.y - 45) / 65;


    if(board_data.select_x == x && board_data.select_y == y)
    {
        printf("Selecting the piece %s\n", piece->bmp_path);
    }
    return true;
}