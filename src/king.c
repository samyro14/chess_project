#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../board.h"
#include "../moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_king_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    int dy[] = {-1, -1, -1, 0, 1, 1, 1, 0},
        dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    for(int i = 0; i < 8; i++){
        if(to_x == from_x + dx[i] && to_y == from_y + dy[i] && (state->board[to_y][to_x].type == EMPTY  || state->board[to_y][to_x].color != state->board[from_y][from_x].color)){
            return 1; // Valid move
        }
    }
    return -1; // Invalid move
}
int is_check(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    // Check if the move puts the king in check
    // This is a simplified version and should be expanded for full functionality
    return 0; // 0 = not in check, 1 = in check
}
int is_checkmate(GameState* state) {
    // Check if the king is in checkmate
    // This is a simplified version and should be expanded for full functionality
    return 0; // 0 = not in checkmate, 1 = in checkmate
}
void move_king(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_king_move(state, from_x, from_y, to_x, to_y) == 1){
        // Move the king
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        state->is_white_turn = !state->is_white_turn;
        return;
        
    } else {
        printf("Invalid move for King\n");
    }
}