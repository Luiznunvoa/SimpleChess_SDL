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

#include <stdio.h>

#include "ui.h"
#include "res.h"
#include "board.h"
#include "pieces.h"

#define BOARD_SIZE 520
#define BOARD_X 40
#define BOARD_Y 40

int element_count; // total number of elements

Element** ui_elements; // array of pointers to structs containing the data from each ui_element

// Initializes the ui elements into the "ui_elements" array
bool ui_init_elements()
{
    if(!create_board())
        return false;

    for(int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (board[y][x] != 0)
                if(!create_piece(x,y, board[y][x]))
                    return false;

    selected_piece = (SelectedPiece){0};

    printf("ui_elements created\n");
    return true;
}

// Calls every update function pointer different than NULL
bool ui_update_elements()
{
    for(int i = 0; i < element_count; i++)
        if(ui_elements[i]->update != NULL)
            if(!ui_elements[i]->update(ui_elements[i]))
                return false;
    return true;
}

// Update the render state of the ui for elements with valid textures
void ui_present()
{
    for (int i = 0; i < element_count; i++)
        if(ui_elements[i]->texture != NULL)
            SDL_RenderCopy(renderer, ui_elements[i]->texture, NULL, &ui_elements[i]->rect);

    SDL_RenderPresent(renderer);
}

// Frees the ui elements in the "ui_elements" array
void ui_free_elements()
{
    for(int i = 0; i < element_count; i++)
        if(ui_elements[i]->texture != NULL)
        {
            SDL_DestroyTexture(ui_elements[i]->texture);
            free(ui_elements[i]);
            ui_elements[i] = NULL;
        }
    free(ui_elements);
    ui_elements = NULL;
    printf("ui_elements freed\n");
}

// helper function to create ui_element of the game board
__forceinline bool create_board()
{
    return ui_create_element(
        (SDL_Rect){BOARD_X, BOARD_Y, BOARD_SIZE, BOARD_SIZE,},
        board_init,
        0
    );
}

// helper function to create the ui_element for each piece in the board
__forceinline bool create_piece(const int x, const int y, const Uint8 type)
{
    return ui_create_element(
        (SDL_Rect){(BOARD_X + 5) + (65 * x), (BOARD_Y + 5) + (65 * y), 0, 0},
        pieces_init,
        type
    );
}

// sets up the initial data of the ui_element
bool ui_create_element( const SDL_Rect rect, const ELM_init init, const Uint8 type)
{
    if(element_count == 0)
        ui_elements = (Element**)malloc(sizeof(Element*));
    else
        ui_elements = (Element**)realloc(ui_elements, (element_count + 1) * sizeof(Element*));

    if(ui_elements == NULL)
        return false;

    ui_elements[element_count] = (Element*)malloc(sizeof(Element));
    *ui_elements[element_count] = (Element){0};

    ui_elements[element_count]->rect = rect;
    ui_elements[element_count]->init = init;
    ui_elements[element_count]->type = type;

    if(!ui_elements[element_count]->init(ui_elements[element_count], renderer))
        return false;

    element_count++;
    return true;
}