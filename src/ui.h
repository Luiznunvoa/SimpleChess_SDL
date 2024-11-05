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

#include "common.h"

typedef struct Element Element;

typedef _Bool (*ELM_init)(Element* element,SDL_Renderer* renderer);
typedef int (*ELM_update)(const Element* element);

typedef struct
{
    int element_count;
    _Bool update;

    Element* elements;
}UIContext;

typedef struct Element
{
    SDL_Texture* texture;
    SDL_Rect rect;

    ELM_init init;
    ELM_update update;

    Uint8 type;

    const char* bmp_path;
}Element;

_Bool init_ui(UIContext* ui, SDL_Renderer* renderer);
_Bool update_ui(UIContext* ui);
void present_ui(const Element* elements, const int element_count,  SDL_Renderer* renderer);
_Bool ui_create_element(UIContext* ui, SDL_Renderer* renderer, SDL_Rect rect, ELM_init init, Uint8 type);
void free_UI(Element* elements, const int element_count);

#endif //UI_H
