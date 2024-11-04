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

#ifndef PIECES_H
#define PIECES_H

#include "common.h"
#include "ui.h"

typedef enum
{
    NOT_A_PIECE,
    BLACK_PAWN,
    WHITE_PAWN,
    BLACK_HORSE,
    WHITE_HORSE,
    BLACK_BISHOP,
    WHITE_BISHOP,
    BLACK_ROOK,
    WHITE_ROOK,
    BLACK_QUEEN,
    WHITE_QUEEN,
    BLACK_KING,
    WHITE_KING
} piece_types;

_Bool pieces_init(Element* piece, SDL_Renderer* renderer);
int pieces_update(const Element* piece);

extern Uint8 piece_board[8][8];

#endif //PIECES_H
