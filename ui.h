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

typedef struct Element
{
    SDL_Texture* texture;
    SDL_Rect rect;

    SDL_Color color1;
    SDL_Color color2;

    ELM_init init;
    ELM_update update;

    const char* bmp_path;
}Element;

bool ui_init_elements();
bool ui_update_elements();
void ui_present();
void ui_free_elements();
bool ui_create_element(
    const int x, const int y, const int w, const int h,
    const SDL_Color color1, const SDL_Color color2,
    const ELM_init init,
    const ELM_update update,
    const char* bmp_path
    );
bool create_piece(const int x, const int y, const char* bmp_path);

extern int element_count;

#endif //UI_H
