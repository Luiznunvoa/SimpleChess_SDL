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

#define COLOR_TILE1 (SDL_Color){119, 107, 93,}
#define COLOR_TILE2 (SDL_Color){155, 145, 134}
#define NO_COLOR  (SDL_Color){0}

#define BOARD_SIZE 520
#define BOARD_X 40
#define BOARD_Y 40

int board[8][8] = {
    7, 3, 5, 9, 11, 5, 3, 7,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2,
    8, 4, 6, 10, 12, 6, 4, 8,
};

int element_count;

Element** ui_elements;
SelectedPiece selected_piece;

bool ui_init_elements()
{
    if(!create_board())
        return false;

    for(int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] != 0)
                if(!create_piece(j,i, board[i][j]))
                    return false;

    printf("ui_elements created\n");
    return true;
}

bool ui_update_elements()
{
    for(int i = 0; i < element_count; i++)
        if(ui_elements[i]->update != NULL)
            if(!ui_elements[i]->update(ui_elements[i]))
                return false;
    return true;
}

void ui_present()
{
    for (int i = 0; i < element_count; i++)
        if(ui_elements[i]->texture != NULL)
            SDL_RenderCopy(renderer, ui_elements[i]->texture, NULL, &ui_elements[i]->rect);

    SDL_RenderPresent(renderer);
}

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

__forceinline bool create_board()
{
    return ui_create_element(
        BOARD_X, BOARD_Y, BOARD_SIZE, BOARD_SIZE,
        COLOR_TILE1, COLOR_TILE2,
        board_init,
        board_update,
        0
    );
}

__forceinline bool create_piece(const int x, const int y, const piece_types type)
{
    return ui_create_element(
        (BOARD_X + 5) + (65 * x), (BOARD_Y + 5) + (65 * y), 0, 0,
        NO_COLOR, NO_COLOR,
        pieces_init,
        pieces_update,
        type
    );
}

bool ui_create_element(
    const int x, const int y, const int w, const int h,
    const SDL_Color color1, const SDL_Color color2,
    const ELM_init init,
    ELM_update update,
    piece_types type
    )
{
    if(element_count == 0)
        ui_elements = (Element**)malloc(sizeof(Element*));
    else
        ui_elements = (Element**)realloc(ui_elements, (element_count + 1) * sizeof(Element*));

    if(ui_elements == NULL)
        return false;

    ui_elements[element_count] = (Element*)malloc(sizeof(Element));

    ui_elements[element_count]->rect.x = x;
    ui_elements[element_count]->rect.y = y;
    ui_elements[element_count]->rect.w = w;
    ui_elements[element_count]->rect.h = h;
    ui_elements[element_count]->color1 = color1;
    ui_elements[element_count]->color2 = color2;
    ui_elements[element_count]->init = init;
    ui_elements[element_count]->update = update;
    ui_elements[element_count]->type = type;
    ui_elements[element_count]->bmp_path = NULL;


    if(!ui_elements[element_count]->init(ui_elements[element_count], renderer))
        return false;

    element_count++;
    return true;
}