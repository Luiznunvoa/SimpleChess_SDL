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
#include "board.h"
#include "pieces.h"

#define FPS 60

// Main game loop, event processing, ui update and frame rate control
void game()
{
    Uint32 start_time = 0; // The tick that the frame started
    Uint32 frame_time = 0; // The time that the frame took to be rendered

    for (bool quit = 0; !quit;)
    {
        quit = event_proc();

        start_time = SDL_GetTicks();

        if(update)
            if(ui_update_elements())
                ui_present();
            else
                quit  = true;

        frame_time = SDL_GetTicks() - start_time;

        if (frame_time < (1000 / FPS))
            SDL_Delay((1000 / FPS) - frame_time); // sleeps through the time remaining to keep the fps stable
    }
}

// Main event processing function, window quit input and keyboard inputs
int event_proc()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYUP:
            return key_input_proc(event.key.keysym.sym);
        default:
        }
    }
    return false;
}

// Keyboard input processing, treating the logic of the arrow keys to move the cursor and the ESC key to close the app
int key_input_proc(const SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_ESCAPE:
        return true;
    case SDLK_UP:
        if(board_data.select_y > 0)
            board_data.select_y -= 1;
        update = true;
        break;
    case SDLK_DOWN:
        if(board_data.select_y < 7)
            board_data.select_y += 1;
        update = true;
        break;
    case SDLK_LEFT:
        if(board_data.select_x > 0)
            board_data.select_x -= 1;
        update = true;
        break;
    case SDLK_RIGHT:
        if(board_data.select_x < 7)
            board_data.select_x += 1;
        update = true;
        break;
    case SDLK_i:
        selected_piece.lock = !selected_piece.lock;
        update = true;
        break;
    default:
    }
    return false;
}