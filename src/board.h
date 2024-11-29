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

#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "ui.h"

_Bool init_board(Element* board, SDL_Renderer* renderer);
int update_board(Element* board, GameContext* game);
void update_selected(int x, int y, int map[8][8], Uint16* pixels, int pitch);
void update_locked(int x, int y, _Bool piece_locked, Uint16* pixels, int pitch);
_Bool setup_texture(SDL_Texture* texture, Uint16** pixels, int* pitch, SDL_PixelFormat** format, Uint32 type);
void draw_rectangle(Uint16* pixels, int pitch, SDL_Rect rect, Uint16 color);

#endif //BOARD_H
