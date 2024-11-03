#include "pieces.h"

Uint8 piece_board[8][8] = {
    {1, 2, 3, 4, 5, 6, 7, 8},   
    {9, 10, 11, 12, 13, 14, 15, 16}, 
    {0, 0, 0, 0, 0, 0, 0, 0},     
    {0, 0, 0, 0, 0, 0, 0, 0},     
    {0, 0, 0, 0, 0, 0, 0, 0},     
    {0, 0, 0, 0, 0, 0, 0, 0},     
    {17, 18, 19, 20, 21, 22, 23, 24}, 
    {25, 26, 27, 28, 29, 30, 31, 32}  
};

_Bool pieces_init(Element* piece, SDL_Renderer* renderer)
{
    typedef struct {
        Uint8 piece_number;
        const char* bmp_path;
    } PieceMap;

    const PieceMap piece_map[] = {
        {1, "../../assets/black_rook.bmp"},
        {2, "../../assets/black_horse.bmp"},
        {3, "../../assets/black_bishop.bmp"},
        {4, "../../assets/black_queen.bmp"},
        {5, "../../assets/black_king.bmp"},
        {6, "../../assets/black_bishop.bmp"},
        {7, "../../assets/black_horse.bmp"},
        {8, "../../assets/black_rook.bmp"},
        {9, "../../assets/black_pawn.bmp"},
        {10, "../../assets/black_pawn.bmp"},
        {11, "../../assets/black_pawn.bmp"},
        {12, "../../assets/black_pawn.bmp"},
        {13, "../../assets/black_pawn.bmp"},
        {14, "../../assets/black_pawn.bmp"},
        {15, "../../assets/black_pawn.bmp"},
        {16, "../../assets/black_pawn.bmp"},
        {17, "../../assets/white_pawn.bmp"},
        {18, "../../assets/white_pawn.bmp"},
        {19, "../../assets/white_pawn.bmp"},
        {20, "../../assets/white_pawn.bmp"},
        {21, "../../assets/white_pawn.bmp"},
        {22, "../../assets/white_pawn.bmp"},
        {23, "../../assets/white_pawn.bmp"},
        {24, "../../assets/white_pawn.bmp"},
        {25, "../../assets/white_rook.bmp"},
        {26, "../../assets/white_horse.bmp"},
        {27, "../../assets/white_bishop.bmp"},
        {28, "../../assets/white_queen.bmp"},
        {29, "../../assets/white_king.bmp"},
        {30, "../../assets/white_bishop.bmp"},
        {31, "../../assets/white_horse.bmp"},
        {32, "../../assets/white_rook.bmp"},
    };

    for (int i = 0; i < sizeof(piece_map) / sizeof(piece_map[0]); i++)
        if (piece->type == piece_map[i].piece_number)
        {
            piece->bmp_path = piece_map[i].bmp_path;
            break;
        }

    SDL_Surface* pieces_surface = SDL_LoadBMP(piece->bmp_path);
    if (!pieces_surface)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error loading the BMP file to the surface: %s\n", SDL_GetError());
        return false;
    }

    piece->texture = SDL_CreateTextureFromSurface(renderer, pieces_surface);

    piece->rect.w = pieces_surface->w;
    piece->rect.h = pieces_surface->h;

    SDL_FreeSurface(pieces_surface);

    if (!piece->texture)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error creating the texture: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
