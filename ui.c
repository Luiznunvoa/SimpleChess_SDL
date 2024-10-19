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

int element_count;

Element* ui_elements;

bool ui_init_elements()
{


    return true;
}

void ui_update_elements()
{
    for(int i = 0; i < element_count; i++)
    {
        if(ui_elements[i].update != NULL)
            ui_elements[i].update(&ui_elements[i]);
    }
}

void ui_free_elements()
{
    for(int i = 0; i < element_count; i++)
    {
        if(ui_elements->texture != NULL)
            SDL_DestroyTexture(ui_elements->texture);
        free(&ui_elements[i]);
    }
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