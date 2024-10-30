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
#include "board.h"

#define BOARD_RECT (SDL_Rect){40, 40, 520, 520}

int element_count = 0;

Element* init_elements(SDL_Renderer** renderer)
{
    Element* elements = NULL;

    if(!create_element(&elements, renderer, BOARD_RECT, board_init, 0))
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create board\n");
        free_UI(&elements);
    };
    return  elements;
}

void free_UI(Element** elements)
{
    for (int i = 0; i < element_count; i++)
    {
        if (elements[i]->texture != NULL)
        {
            SDL_DestroyTexture(elements[i]->texture);
            elements[i]->texture = NULL;
        }
    }
    free(*elements);
    *elements = NULL;
    element_count = 0;
}

int update_UI(Element** elements)
{
    int result = false;

    for(int i = 0; i < element_count; i++)
    {
        if(*elements[i]->update != NULL)
        {
            result = elements[i]->update(elements[i]);

            if(result == error)
            {
                SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error updating UI element\n");
                return error;
            }

            if(result == true)
                result = true;
        }
    }
    return result;
}

void present_UI(Element* elements, SDL_Renderer** renderer)
{
    for (int i = 0; i < element_count; i++)
        if(elements[i].texture != NULL)
            SDL_RenderCopy(*renderer, elements[i].texture, NULL, &elements[i].rect);

    SDL_RenderPresent(*renderer);
}

_Bool create_element(Element** elements, SDL_Renderer** renderer, const SDL_Rect rect, const ELM_init init, const Uint8 type)
{
    Element* temp_elements = NULL;

    if (*elements == NULL)
        temp_elements = (Element*)malloc(sizeof(Element));
    else
        temp_elements = (Element*)realloc(elements, (element_count + 1) * sizeof(Element));

    if (temp_elements == NULL)
        return false;

    *elements = temp_elements;
    temp_elements = NULL;

    *elements[element_count] = (Element){0};

    elements[element_count]->rect = rect;
    elements[element_count]->init = init;
    elements[element_count]->type = type;

    if(elements[element_count]->init == NULL)
        return false;

    if(!elements[element_count]->init(elements[element_count], renderer))
        return false;

    element_count++;
    return true;
}