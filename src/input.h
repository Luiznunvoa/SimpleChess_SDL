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

#ifndef INPUT_H
#define INPUT_H

#include "common.h"

Uint32 key_input(const SDL_Keycode keycode, int* cursor_x, int* cursor_y, _Bool* piece_locked, _Bool* update);
Uint32 mouse_input(int* cursor_x, int* cursor_y, int lock_x, int lock_y,_Bool* piece_locked, _Bool* update);

#endif //INPUT_H
