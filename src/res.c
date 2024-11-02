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

#include "res.h"

#define GAME_TITLE "SimpleChess"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_X SDL_WINDOWPOS_CENTERED
#define WINDOW_Y SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN)
#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#define BACKGROUND_COLOR 143, 138, 134, 1

_Bool init(SDL_Renderer** renderer, SDL_Window** window)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"SDL Initialization error: %s\n", SDL_GetError());
        return false;
    }

    *window  = SDL_CreateWindow(
        GAME_TITLE,
        WINDOW_X, WINDOW_Y,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        WINDOW_FLAGS
        );

    if (*window == NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Window initialization error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* iconSurface = SDL_LoadBMP("../../assets/black_pawn.bmp");

    if (iconSurface != NULL)
    {
        SDL_SetWindowIcon(*window, iconSurface);
        SDL_FreeSurface(iconSurface);
        iconSurface = NULL;
    }
    else
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Error loading icon: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        RENDERER_FLAGS
        );

    if (*renderer == NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Failure in the renderer initialization: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(*renderer, BACKGROUND_COLOR);

    SDL_RenderClear(*renderer);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window Resources Initialized");

    return true;
}

void quit(SDL_Renderer** renderer, SDL_Window** window)
{
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window Resources Deallocated");
    SDL_Quit();
}