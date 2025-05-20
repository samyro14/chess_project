#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../src/board.h"
#include "../src/moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_queen_move(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_rook_move(state, from_x, from_y, to_x, to_y) == -1 && is_valid_bishop_move(state, from_x, from_y, to_x, to_y) == -1){
        return -1; // Invalid move
    }
    return 1; // valid move
}
void move_queen(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_queen_move(state, from_x, from_y, to_x, to_y) == 1){
        Piece temp_piece = state->board[to_y][to_x];
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        if(is_check(state)){
            state->board[from_y][from_x] = state->board[to_y][to_x];
            state->board[to_y][to_x] = temp_piece;
            printf("Invalid move! King is in check!\n");
            return;
        }
        state->is_white_turn = !state->is_white_turn;
    }
    else{
        printf("Invalid move for queen!\n");  
    }
}