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

_Bool init_ui(UIContext* ui, SDL_Renderer** renderer)
{
    *ui = (UIContext){0};
    ui->update = true;

    if(!ui_create_element(ui, renderer, BOARD_RECT, board_init, 0))
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create board\n");
        free_UI(ui);
        return false;
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "UI Elements Initialized");
    return true;
}

void free_UI(UIContext* ui)
{
    for(int i = 0; i < ui->element_count; i++)
        if(ui->elements[i].texture != NULL)
        {
            SDL_DestroyTexture(ui->elements[i].texture);

        }
    free(ui->elements);
    ui->elements = NULL;

    ui->element_count = 0;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "UI Elements Deallocated");
}

_Bool UI(UIContext* ui, SDL_Renderer** renderer)
{
    while(ui->update)
    {
        ui->update = false;

        const int result = update_ui(ui);

        if(result < 0)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to update UI\n");
            return false;
        }

      if(result == true)
            ui->update = true;

        present_ui(ui, renderer);
    }
    return true;
}

int update_ui(UIContext* ui)
{
    int result = false;
    for(int i = 0; i < ui->element_count; i++)
        if(ui->elements[i].update != NULL)
            switch (ui->elements[i].update(&ui->elements[i]))
            {
            case error:
                return error;
            case true:
                result = true;
            default:
                continue;
            }
    return result;
}

void present_ui(UIContext* ui, SDL_Renderer** renderer)
{
    for (int i = 0; i < ui->element_count; i++)
        if(ui->elements[i].texture != NULL)
            SDL_RenderCopy(*renderer, ui->elements[i].texture, NULL, &ui->elements[i].rect);

    SDL_RenderPresent(*renderer);
}

_Bool ui_create_element(UIContext* ui, SDL_Renderer** renderer, const SDL_Rect rect, const ELM_init init, const Uint8 type)
{
    Element* temp_elements;

    if(ui->element_count == 0)
        temp_elements = (Element*)malloc(sizeof(Element));
    else
        temp_elements = (Element*)realloc(ui->elements, (ui->element_count + 1) * sizeof(Element));

    if(temp_elements == NULL)
        return false;

    ui->elements = temp_elements;
    ui->elements[ui->element_count] = (Element){0};

    ui->elements[ui->element_count].rect = rect;
    ui->elements[ui->element_count].init = init;
    ui->elements[ui->element_count].type = type;

    if(!ui->elements[ui->element_count].init(&ui->elements[ui->element_count], renderer))
        return false;

    ui->element_count++;
    return true;
}

void refresh_ui(UIContext* ui)
{
    ui->update = true;
}