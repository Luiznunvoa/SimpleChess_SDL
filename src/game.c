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
#include "res.h"

void game()
{
    GameContext game = (GameContext){0};

    WindowContext res;

    if (!init(&res))    // Initialize the SDL resources
    {
        SDL_ShowSimpleMessageBox(16, "ERROR", "Failed to Initialize Window", res.window);
        quit(&res);
        return;
    }

    UIContext ui;

    if (!init_ui(&ui, res.renderer, &game.board))   // Initialize the UI elements
    {
        SDL_ShowSimpleMessageBox(16, "ERROR", "Failed to Initialize UI", res.window);
        quit(&res);
        return;
    }

    while (SDL_WaitEvent(&game.event) && game.flag != QUIT_GAME) // Main game loop
    {
        switch (game.event.type)
        {
        case SDL_QUIT: // User closed the window
            game.flag = QUIT_GAME;
            break;

        case SDL_KEYUP: // User pressed and released a key
            ui.update = true;

            game.flag = key_input_proc(game.event.key.keysym.sym, &game.cursor_x, &game.cursor_y);
            break;

        case SDL_MOUSEBUTTONUP:  // User pressed and released the mouse button
            ui.update = true;
            mouse_input_proc(&game.cursor_x, &game.cursor_y);
            break;

        default: // Unhandled events
            ui.update = false; // Do not update UI
            game.flag = DEFAULT; // Reset game flag to default
        }
        get_ui(&ui, &game, res.renderer);
    }
    // Cleanup
    free_ui(ui.elements);
    quit(&res);
}