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
#include "pieces.h"

#define BOARD_X 40
#define BOARD_Y 40
#define BOARD_RECT (SDL_Rect){BOARD_X, BOARD_Y, 520, 520}
#define PIECE_RECT(x, y) (SDL_Rect){(BOARD_X + 5) + (65 * x), (BOARD_Y + 5) + (65 * y), 0, 0}

_Bool init_ui(UIContext* ui, SDL_Renderer* renderer, int board_map[8][8])
{
    *ui = (UIContext){0};
    ui->update = true;

    if(!ui_create_element(&ui->elements, renderer, BOARD_RECT, board_init, 0))
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create board\n");
        free_ui(ui->elements);
        return false;
    }

    for(int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (board_map[y][x] != 0)
                if(!ui_create_element(&ui->elements, renderer, PIECE_RECT(x, y), pieces_init, board_map[y][x]))
                    return false;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "UI Elements Initialized");
    return true;
}

void free_ui(Element* elements)
{
    Element* current = elements;
    while (current != NULL)
    {
        Element* next = current->next;

        if (current->texture != NULL)
        {
            SDL_DestroyTexture(current->texture);
            current->texture = NULL;
        }

        free(current);
        current = NULL;

        current = next;
    }

    elements = NULL;

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "UI Elements Deallocated");
}

_Bool update_ui(UIContext* ui, GameContext* game)
{
    const Element* current = ui->elements;

    ui->update = false;

    while (current != NULL)
    {
        if (current->update != NULL)
        {
            const int update_result = current->update(current, game);

            switch (update_result)
            {
            case error:
                return false;
            case true:
                ui->update = true;
                break;
            case false:
                break;
            case 2:
                {
                    const Element* element_to_delete = current;
                    current = current->next;

                    ui_delete_element( &(ui->elements), &ui->update,  &game->board_map, element_to_delete->info);

                    continue;
                }
            default:
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Weird update return");
            }
        }

        current = current->next;
    }

    return true;
}

void present_ui(const Element* elements, SDL_Renderer* renderer)
{
    const Element* current = elements;
    while (current != NULL)
    {
        if (current->texture != NULL)
            SDL_RenderCopy(renderer, current->texture, NULL, &current->rect);

        current = current->next;
    }

    SDL_RenderPresent(renderer);
}

_Bool ui_create_element(
    Element** elements,
    SDL_Renderer* renderer,
    const SDL_Rect rect,
    const ELM_init init,
    const Uint8 info
)
{
    Element* new_element = (Element*)malloc(sizeof(Element));

    if (!new_element)
        return false;

    *new_element = (Element){0};
    new_element->rect = rect;
    new_element->init = init;
    new_element->info = info;

    if(!new_element->init(new_element, renderer))
    {
        free(new_element);
        return false;
    }

    if (*elements == NULL)
    {
        *elements = new_element;
    }
    else
    {
        Element* temp_elements = *elements;
        while(temp_elements->next != NULL)
        {
            temp_elements = temp_elements->next;
        }
        temp_elements->next = new_element;
    }

    return true;
}

void ui_delete_element(
    Element** elements,
    _Bool* update,
    int(*board_map)[8][8],
    const Uint8 info
)
{
    if (elements == NULL || *elements == NULL)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "No element to delete");
        return;
    }

    Element* current = *elements;
    Element* previous = NULL;

    while (current != NULL)
    {
        if (current->info == info)
        {
            if (previous == NULL)
                *elements = current->next;
            else
                previous->next = current->next;

            if (current->texture != NULL)
            {
                SDL_DestroyTexture(current->texture);
                current->texture = NULL;
            }

            const int board_x = current->rect.x - (BOARD_X + 5);
            const int board_y = current->rect.y - (BOARD_Y + 5);

            (*board_map)[board_y / 65][board_x / 65] = 0;

            *update = true;

            free(current);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Element %d deleted", info);
            return;
        }
        previous = current;
        current = current->next;
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Element with info %d not found", info);
}


