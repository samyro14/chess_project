#ifndef BOARD_H
#define BOARD_H
#include <SDL2/SDL.h>
#define SDL_MAIN_HANDLED
#define BOARD_SIZE 8
typedef enum {
    EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
} PieceType;

typedef enum {
    WHITE, BLACK
} PieceColor;

typedef struct {
    PieceType type;
    PieceColor color;
} Piece;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Piece board[BOARD_SIZE][BOARD_SIZE];
    int is_white_turn;
    int selected_x;
    int selected_y;
} GameState;


#endif /* board_h */