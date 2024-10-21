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
#define COLOR_TILE2 (SDL_Color){155, 145, 134}

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

Element* ui_elements;

bool ui_init_elements()
{
    if(!ui_create_element(
        140, 20, 520, 520,
        COLOR_TILE1,
        COLOR_TILE2,
        board_init,
        board_update,
        NULL
        ))
        return false;

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            switch (board[i][j])
            {
            case 1:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/pawn.bmp"
                    ))
                    return false;
                break;

            case 2:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/pawn2.bmp"
                    ))
                    return false;
                break;
            case 3:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/horse.bmp"
                    ))
                    return false;
                    break;

            case 4:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/horse2.bmp"
                    ))
                    return false;
                    break;
            case 7:
                if(!ui_create_element(
                    (145 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/rook.bmp"
                    ))
                    return false;
                break;
            case 8:
                if(!ui_create_element(
                    (145 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/rook2.bmp"
                    ))
                    return false;
                    break;
            case 5:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/bishop.bmp"
                    ))
                    return false;
                    break;
            case 6:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/bishop2.bmp"
                    ))
                    return false;
                    break;

            case 9:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/queen.bmp"
                    ))
                    return false;
                    break;
            case 10:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/queen2.bmp"
                    ))
                    return false;
                    break;
            case 11:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/king.bmp"
                    ))
                    return false;
                    break;
            case 12:
                if(!ui_create_element(
                    (143 + (65 * j)), (25 + (65 * i)), 0, 0,
                    COLOR_TILE1,
                    COLOR_TILE2,
                    pieces_init,
                    NULL,
                    "../../assets/king2.bmp"
                    ))
                    return false;
                    break;

            // TODO OTHER PIECES CASES
            default:
                //return false;
            }

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
    ELM_update update,
    char* bmp_path
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
    ui_elements[element_count].bmp_path = bmp_path;


    if(!ui_elements[element_count].init(&ui_elements[element_count], renderer))
        return false;

    element_count++;
    return true;
}