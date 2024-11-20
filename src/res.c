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

// Initializes the SDL resources
_Bool init(WindowContext* res)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"SDL Initialization error: %s\n", SDL_GetError());
        return false;
    }

    res->window  = SDL_CreateWindow(
        "SimpleChess",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
        );

    if (res->window == NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Window initialization error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* iconSurface = SDL_LoadBMP("../../assets/black_pawn.bmp");

    if (iconSurface != NULL)
    {
        SDL_SetWindowIcon(res->window, iconSurface);
        SDL_FreeSurface(iconSurface);
        iconSurface = NULL;
    }
    else
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Error loading icon: %s\n", SDL_GetError());
        return false;
    }

    res->renderer = SDL_CreateRenderer(
        res->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

    if (res->renderer == NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Failure in the renderer initialization: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(res->renderer, 143, 138, 134, 1);

    SDL_RenderClear(res->renderer);

    SDL_RenderPresent(res->renderer);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window Resources Initialized");

    return true;
}

// Frees the SDL resources
void quit(const WindowContext* res)
{
    SDL_DestroyRenderer(res->renderer);
    SDL_DestroyWindow(res->window);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window Resources Deallocated");
    SDL_Quit();
}