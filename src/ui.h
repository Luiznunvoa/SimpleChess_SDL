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

typedef struct Element Element; // Forward declaration of the Element structure

// Typedef for initialization function for an Element
typedef _Bool (*ELM_init)(Element* element, SDL_Renderer* renderer);
typedef int (*ELM_update)(Element* element, GameContext* game);

typedef struct
{
    _Bool update;       // Indicates whether the UI needs updating
    Element* elements;  // Linked list of UI elements
} UIContext;

typedef struct Element // Structure to represent a UI element
{
    SDL_Texture* texture;    // Texture associated with the element
    SDL_Rect rect;           // Rectangle defining the element's position and size

    ELM_init init;           // Function pointer for element initialization
    ELM_update update;       // Function pointer for element updates

    Uint8 info;              // Additional information or identifier for the element

    Element* next;           // Pointer to the next element in the linked list
} Element;

void get_ui(UIContext* ui, GameContext* game);
_Bool init_ui(UIContext* ui, SDL_Renderer* renderer, int(*board_map)[8][8]);
void free_ui(Element* elements);
_Bool update_ui(UIContext* ui, GameContext* game);
void present_ui(const Element* elements, SDL_Renderer* renderer);
_Bool create_element(Element** elements, SDL_Renderer* renderer, SDL_Rect rect, ELM_init init, Uint8 info);
void delete_element(Element** elements,_Bool* update, int(*board_map)[8][8], Uint8 info);

#endif // UI_H