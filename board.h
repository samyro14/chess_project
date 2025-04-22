#ifndef BOARD_H
#define BOARD_H
#include <SDL2/SDL.h>
#define SDL_MAIN_HANDLED
#define BOARD_SIZE 8


/*char board[8][8] = {
    { 'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'},
    { 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    { 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    { 'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'}
    };*/
typedef enum{
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

void init_game(GameState*);
void render_board(GameState*);
void reder_piece(GameState*, const char*, int, int);


#endif /* board_h */