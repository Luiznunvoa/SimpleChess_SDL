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

#include "pieces.h"

bool pieces_init(Element* pieces, SDL_Renderer* renderer) {
    void* pixels;
    int pitch;

    pieces->texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        pieces->rect.w, pieces->rect.h
    );

    if (!pieces->texture) {
        printf("Error creating texture: %s\n", SDL_GetError());
        return false;
    }

    if (SDL_LockTexture(pieces->texture, NULL, &pixels, &pitch) < 0) {
        printf("Error locking texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(pieces->texture);
        return false;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
    if (!format) {
        printf("Error allocating format: %s\n", SDL_GetError());
        SDL_UnlockTexture(pieces->texture);
        SDL_DestroyTexture(pieces->texture);
        return false;
    }

    Uint32 color = SDL_MapRGBA(format, 0, 0, 0, 0);

    Uint32* pixel_ptr = (Uint32*)pixels;

    for (int i = 0; i < pieces->rect.h; i++)
        for (int j = 0; j < pieces->rect.w; j++)
            pixel_ptr[j + i * (pitch / 4)] = color;

    SDL_FreeFormat(format);
    SDL_UnlockTexture(pieces->texture);

    SDL_SetTextureBlendMode(pieces->texture, SDL_BLENDMODE_BLEND);

    return true;
}