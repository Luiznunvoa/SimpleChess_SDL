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

// Macro for the board's SDL Rectangle
#define BOARD_RECT (SDL_Rect){OFFSET_X, OFFSET_Y, FIELD_W, FIELD_H}

// Macro to calculate the piece position in the board and create it's SDL_Rect struct
#define PIECE_RECT(x, y) (SDL_Rect){OFFSET_X + (CELL_W * x), OFFSET_Y + (CELL_H * y), 0, 0}

// Function to update and present of the UI
void get_ui(UIContext* ui, GameContext* game)
{
    while (ui->update && game->flag != NOTHING) // While the UI needs updating and the game is not quitting
    {
        // If updating the UI succeeds, present the updated elements
        if (update_ui(ui, game))
            present_ui(ui->elements, game->renderer);
        else
        {
            SDL_ShowSimpleMessageBox(16, "ERROR", "Failed to Update UI", game->window);
            game->flag = QUIT_GAME;
            return;
        }
    }
}

// Function to initialize the UI elements
_Bool init_ui(UIContext* ui, SDL_Renderer* renderer, int(*board_map)[8][8])
{
    if (!create_element(&ui->elements, renderer, BOARD_RECT, init_board, 0)) // Create the board element
    {
        SDL_LogCritical(0, "Failed to create board\n");
        free_ui(ui->elements);
        return false;
    }

    const int temp_board_map[8][8] = // Define initial positions for all pieces on the board
    {
        {1, 2, 3, 4, 5, 6, 7, 8},
        {9, 10, 11, 12, 13, 14, 15, 16},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {17, 18, 19, 20, 21, 22, 23, 24},
        {25, 26, 27, 28, 29, 30, 31, 32}
    };

    for (int y = 0; y < 8; y++) // Create piece elements based on the initial board map
        for (int x = 0; x < 8; x++)
            if (temp_board_map[y][x] != 0)
            {   // Create element for each piece
                if (!create_element(&ui->elements, renderer, PIECE_RECT(x, y), init_pieces, temp_board_map[y][x]))
                    return false;
                else
                    (*board_map)[y][x] = temp_board_map[y][x]; // Update the board map
            }
    ui->update = true; // Mark UI as needing an update
    SDL_LogInfo(0, "UI Elements Initialized");
    return true;
}

// Function to free all UI elements
void free_ui(Element* elements)
{
    Element* current = elements;

    while (current != NULL) // Iterate through all elements and free them
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
    SDL_LogInfo(0, "UI Elements Deallocated");
}

// Function to update the UI elements
_Bool update_ui(UIContext* ui, GameContext* game)
{
    Element* current = ui->elements;
    ui->update = false;

    while (current != NULL) // Process updates for each UI element
    {
        Element* next_element = current->next;

        if (current->update != NULL) // Verify if the function pointer is not NULL
        {
            switch (current->update(current, game))
            {
            case error: // Error in update
                return false;

            case false: // No other update needed
                break;

            case true: // Other update needed
                ui->update = true;
                break;

            case 2: // Delete the element
                delete_element(&(ui->elements), &ui->update, &game->board, current->info);
                break;

            case 3: // Game over (checkmate)
                SDL_ShowSimpleMessageBox(64, "CHECKMATE", "GAME OVER", game->window);
                game->flag = QUIT_GAME;
                break;

            default: // Unexpected update result
                SDL_LogWarn(0, "Weird update return");
            }
        }
        current = next_element;
    }
    return true;
}

// Function to render all UI elements to the screen
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

// Function to create a new UI element
_Bool create_element(Element** elements, SDL_Renderer* renderer, const SDL_Rect rect, const ELM_init init, const Uint8 info)
{
    Element* new_element = (Element*)malloc(sizeof(Element));

    if (!new_element)
        return false;

    // Initialization of the new element data
    *new_element = (Element){0};
    new_element->rect = rect;
    new_element->init = init;
    new_element->info = info;
    new_element->next = NULL;

    if (!new_element->init(new_element, renderer))
    {
        free(new_element);
        return false;
    }

    if (*elements == NULL) // First element
        *elements = new_element;
    else // Add to the end of the list
    {
        Element* temp_elements = *elements;

        while (temp_elements->next != NULL)
            temp_elements = temp_elements->next;

        temp_elements->next = new_element;
    }
    return true;
}

// Function to delete a specific UI element
void delete_element(Element** elements, _Bool* update, int(*board_map)[8][8], const Uint8 info)
{
    if (elements == NULL || *elements == NULL)
    {
        SDL_LogWarn(0, "No element to delete");
        return;
    }

    Element* current = *elements;
    Element* previous = NULL;

    while (current != NULL)
    {
        if (current->info == info)
        {
            if (previous == NULL) // First element in the list
                *elements = current->next;
            else
                previous->next = current->next;

            if (current->texture != NULL)
            {
                SDL_DestroyTexture(current->texture);
                current->texture = NULL;
            }

            const int board_x = ((current->rect.x - OFFSET_X)  / CELL_W);
            const int board_y = ((current->rect.y - OFFSET_Y) / CELL_H);

            // Clear the board map position
            (*board_map)[board_y][board_x] = 0;
            *update = true;

            free(current);
            SDL_LogInfo(0, "Element %d deleted", info);
            return;
        }
        previous = current;
        current = current->next;
    }
    SDL_LogWarn(0, "Element with info %d not found", info);
}