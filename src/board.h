#ifndef BOARD_H
#define BOARD_H
#include <SDL2/SDL.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
#define BOARD_SIZE 8
#define MAX_NAME_LENGTH 100

typedef enum{
    EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
} PieceType;

typedef enum {
    WHITE, BLACK
} PieceColor;

typedef struct{
    char name_player_1[MAX_NAME_LENGTH];
    char name_player_2[MAX_NAME_LENGTH];
    bool is_ready_player_1;
    bool is_ready_player_2;
}Player;

typedef struct {
    PieceType type;
    PieceColor color;
} Piece;
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Piece board[BOARD_SIZE][BOARD_SIZE];
    Player player;
    int is_white_turn;
    int selected_x; 
    int selected_y;
} GameState;
typedef enum{
    MENU, 
    GAME,
    EXIT_APP
}App_State;

typedef struct{
    SDL_Rect rect;
    SDL_Color color_normal;
    SDL_Color color_hover;
    const char* text;
    bool hovered;
}Button;

void init_game(GameState*, Button*, Button*, Button*);
void render_board(GameState*);
void reder_piece(GameState*, const char*, int, int);
void menu(GameState*, SOCKET*,bool, App_State*, Button*, Button*, Button*);

#endif /* board_h */