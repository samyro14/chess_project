#ifndef MOVES_H
#define MOVES_H
#include "board.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
bool is_valid_move(GameState*, int, int, int, int);
void handle_move(GameState*, int, int, int, int);
void move_pawn(GameState*, int, int, int, int);
void move_knight(GameState*, int, int, int, int);
void move_bishop(GameState*, int, int, int, int);
void move_rook(GameState*, int, int, int, int);
void move_queen(GameState*, int, int, int, int);
void move_king(GameState*, int, int, int, int);

//moves
int is_valid_rook_move(GameState*, int, int, int, int); // 1 valid, -1 invalid
int is_valid_bishop_move(GameState*, int, int, int, int); // 1 valid, -1 invalid
int is_check(GameState*, int, int, int, int); // 1 valid, -1 invalid
int is_checkmate(GameState*); // 1 valid, -1 invalid
#endif
