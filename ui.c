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

#include "ui.h"
#include "res.h"
#include "board.h"
#include "pieces.h"

#define COLOR_BLACK (SDL_Color){0, 0, 0, 255}
#define COLOR_WHITE (SDL_Color){255, 255, 255, 255}
#define COLOR_TILE1 (SDL_Color){119, 107, 93,}
#define COLOR_TILE2 (SDL_Color){255, 245, 234}

int element_count;

Element* ui_elements;

bool ui_init_elements()
{
    if(!ui_create_element(
        140, 20, 520, 520,
        COLOR_TILE1,
        COLOR_TILE2,
        board_init,
        board_update
        ))
        return false;

    if(!ui_create_element(
        140, 20, 530, 530,
        COLOR_TILE1,
        COLOR_TILE2,
        pieces_init,
        NULL
        ))
        return false;

    return true;
}

bool ui_update_elements()
{
    for(int i = 0; i < element_count; i++)
        if(ui_elements[i].update != NULL)
            if(!ui_elements[i].update(&ui_elements[i]))
                return false;
    return true;
}

void ui_present()
{
    for (int i = 0; i < element_count; i++)
        if(ui_elements[i].texture != NULL)
            SDL_RenderCopy(renderer, ui_elements[i].texture, NULL, &ui_elements[i].rect);
    SDL_RenderPresent(renderer);
}

void ui_free_elements()
{
    for(int i = 0; i < element_count; i++)
        if(ui_elements[i].texture != NULL)
            SDL_DestroyTexture(ui_elements[i].texture);
    free(ui_elements);
}

bool ui_create_element(
    int x, int y, int w, int h,
    SDL_Color color1, SDL_Color color2,
    ELM_init init,
    ELM_update update
    )
{
    if(element_count == 0)
        ui_elements = (Element*)malloc(sizeof(Element));
    else
        ui_elements = (Element*)realloc(ui_elements, (element_count + 1) * sizeof(Element));

    if(ui_elements == NULL)
        return false;

    ui_elements[element_count].rect.x = x;
    ui_elements[element_count].rect.y = y;
    ui_elements[element_count].rect.w = w;
    ui_elements[element_count].rect.h = h;
    ui_elements[element_count].color1 = color1;
    ui_elements[element_count].color2 = color2;
    ui_elements[element_count].init = init;
    ui_elements[element_count].update = update;


    if(!ui_elements[element_count].init(&ui_elements[element_count], renderer))
        return false;

    element_count++;
    return true;
}