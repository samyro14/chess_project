#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../board.h"
#include "../moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_bishop_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);
    if(dx != dy)
        return -1; // Not a diagonal move
    if(state->board[to_y][to_x].type != EMPTY && state->board[to_y][to_x].color == state->board[from_y][from_x].color) {
        return -1; // Cannot capture own piece
    }
    // Check for obstacles in the path
    int step_x = to_x > from_x ? 1 : -1;
    int step_y = to_y > from_y ? 1 : -1;
    for(int i = 1; i < dx; ++i){
        int x = from_x + i * step_x;
        int y = from_y + i * step_y;
        if(state->board[y][x].type != EMPTY) {
            return -1; // Obstacle in the way
        }
    }

    return 1; // Valid move
}
void move_bishop(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_bishop_move(state, from_x, from_y, to_x, to_y) == 1){
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        state->is_white_turn = !state->is_white_turn;
    }
    else{
        printf("Invalid move for bishop!\n");  
    }
}