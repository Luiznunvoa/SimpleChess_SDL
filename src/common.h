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

#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>

#define true    1
#define false   0
#define error   -1

#define WINDOW_W 600
#define WINDOW_H 600

#define FIELD_W 520
#define FIELD_H 520

#define OFFSET_X ((WINDOW_W - FIELD_W) / 2)
#define OFFSET_Y ((WINDOW_H - FIELD_H) / 2)

#define CELL_W (FIELD_W / 8)
#define CELL_H (FIELD_H / 8)

#define RECT(x, y, w, h) (SDL_Rect){x, y, w, h}

#endif //COMMON_H
