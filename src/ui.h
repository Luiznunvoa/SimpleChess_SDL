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
#include "game.h"

typedef struct Element Element;

typedef _Bool (*ELM_init)(Element* element,SDL_Renderer* renderer);
typedef int (*ELM_update)(Element const* element, GameContext* game);

typedef struct
{
    _Bool update;

    Element* elements;
}UIContext;

typedef struct Element
{
    SDL_Texture* texture;
    SDL_Rect rect;

    ELM_init init;
    ELM_update update;

    Uint8 info;

    const char* bmp_path;

    Element* next;
}Element;

_Bool init_ui(UIContext* ui, SDL_Renderer* renderer, int(*board_map)[8][8]);
void free_ui(Element* elements);
_Bool update_ui(UIContext* ui, GameContext* game);
void present_ui(const Element* elements, SDL_Renderer* renderer);
_Bool create_element(
    Element** elements,
    SDL_Renderer* renderer,
    const SDL_Rect rect,
    const ELM_init init,
    const Uint8 info
);
void delete_element(
    Element** elements,
    _Bool* update,
    int(*board_map)[8][8],
    const Uint8 info
);

#endif //UI_H
