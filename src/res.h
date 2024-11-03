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

#ifndef RES_H
#define RES_H

#include "common.h"

typedef struct
{
    SDL_Renderer* renderer;
    SDL_Window* window;

    Uint32 start_time; // The tick that the frame started
    Uint32 frame_time; // The time that the frame took to be rendered
}WindowContext;

_Bool init(WindowContext* res);
void quit(const WindowContext* res);

#endif //RES_H
