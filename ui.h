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

#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <stdbool.h>

typedef struct Element Element;

typedef bool (*ELM_init)(Element* element, SDL_Renderer* renderer);
typedef bool (*ELM_update)(Element* element);

typedef enum
{
    NONE,
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
}piece_types;

typedef struct
{
    int x;
    int y;
    piece_types type;
}SelectedPiece;

typedef struct Element
{
    SDL_Texture* texture;
    SDL_Rect rect;

    SDL_Color color1;
    SDL_Color color2;

    ELM_init init;
    ELM_update update;

    piece_types type;
    const char* bmp_path;
}Element;

bool ui_init_elements();
bool ui_update_elements();
void ui_present();
void ui_free_elements();
bool create_board();
bool create_piece(int x, int y, piece_types type);
bool ui_create_element(
    int x, int y, int w, int h,
    SDL_Color color1, SDL_Color color2,
    ELM_init init,
    ELM_update update,
    piece_types type
    );

extern int board[8][8];
extern SelectedPiece selected_piece;

#endif //UI_H
