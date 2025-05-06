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
int is_valid_knight_move(GameState*, int, int, int, int); // 1 validd, -1 invalid
int is_valid_pawn(GameState*, int, int, int, int); //  2 - valid capture from left, 3 - from right, -1 invalid move
int is_valid_queen_move(GameState*, int, int, int, int); // 1 valid move, -1 invalid move
int is_check(GameState*); // 1 - in check, 0 - not in check
int is_checkmate(GameState*); // 1- in checkmate, 0 - not in checkmate
int draw(GameState*); // 1 valid, -1 invalid
#endif
