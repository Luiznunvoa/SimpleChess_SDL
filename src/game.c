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

#include "game.h"
#include "ui.h"
#include "input.h"

void game()
{
    GameContext game = (GameContext){0};

    if (!init(&game.renderer, &game.window)) // Initialize the SDL resources
    {
        SDL_ShowSimpleMessageBox(16, "ERROR", "Failed to Initialize Window", game.window);
        quit(&game.renderer, &game.window);
        return;
    }

    UIContext ui = (UIContext){0};

    if (!init_ui(&ui, game.renderer, &game.board)) // Initialize the UI elements
    {
        SDL_ShowSimpleMessageBox(16, "ERROR", "Failed to Initialize UI", game.window);
        quit(&game.renderer, &game.window);
        return;
    }

    while (SDL_WaitEvent(&game.event) && game.flag != QUIT_GAME) // Main game loop
    {
        switch (game.event.type)
        {
        case SDL_QUIT: // User pressed the close  window button
            game.flag = QUIT_GAME;
            break;

        case SDL_KEYUP: // User pressed and released a key
            ui.update = true;
            game.flag = key_input_proc(game.event.key.keysym.sym, &game.cursor_x, &game.cursor_y);
            break;

        case SDL_MOUSEBUTTONUP: // User pressed and released the mouse button
            ui.update = true;
            game.flag = mouse_input_proc(&game.cursor_x, &game.cursor_y);
            break;

        default: // Unhandled events
            ui.update = false; // Do not update UI
            game.flag = DEFAULT; // Reset game flag to default
        }
        get_ui(&ui, &game);
    }
    // Cleanup
    free_ui(ui.elements);
    quit(&game.renderer, &game.window);
}

// Initializes the SDL resources
_Bool init(SDL_Renderer** renderer, SDL_Window** window)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"SDL Initialization error: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow(
        "SimpleChess",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (*window == NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Window initialization error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* iconSurface = SDL_LoadBMP("../../assets/black_pawn.bmp"); // Loading the window icon
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
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (*renderer == NULL)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Failure in the renderer initialization: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(*renderer, 143, 138, 134, 1);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window Resources Initialized");

    return true;
}

// Frees the SDL resources
void quit(SDL_Renderer** renderer, SDL_Window** window)
{
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Window Resources Deallocated");
    SDL_Quit();
}