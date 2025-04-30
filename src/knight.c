#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../board.h"
#include "../moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_knight_move(GameState* state,int from_x, int from_y, int to_x, int to_y) {
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);
    return ( ( (dx == 2 && dy == 1) || (dx == 1 && dy == 2) ) && (state->board[to_y][to_x].type == EMPTY || 
            state->board[to_y][to_x].color != state->board[from_y][from_x].color)); // sa fie valida mutarea si sa nu fie ocupata de o piesa de aceeasi culoare
}

void move_knight(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_knight_move(state,from_x, from_y, to_x, to_y) == 1){
        // Mutare piesă
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        state->is_white_turn = !state->is_white_turn;
    }
    else{
        printf("Invalid move for knight!\n");
    }

}