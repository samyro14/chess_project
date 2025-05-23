#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../src/board.h"
#include "../src/moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_rook_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    if(state->board[from_y][from_x].type != ROOK && state->board[from_y][from_x].type != QUEEN) {
        return -1; // invalid piece type
    }

    //  verifying if the move is either horizontal or vertical
    if (from_x != to_x && from_y != to_y) {
        return -1; // Mutare invalidă
    }
    
    // path verification
    if (from_x == to_x) { // vertical move
        int step = (to_y > from_y) ? 1 : -1;
        for (int i = from_y + step; i != to_y; i += step) {
            if (state->board[i][from_x].type != EMPTY) {
                return -1; // path is not clear
            }
        }
    } else if (from_y == to_y) { // horizontal move
        int step = (to_x > from_x) ? 1 : -1;
        for (int i = from_x + step; i != to_x; i += step) {
            if (state->board[from_y][i].type != EMPTY) {
                return -1; // path is not clear
            }
        }
    }

    // verifying if the destination square is occupied by a piece of the same color
    if (state->board[to_y][to_x].type != EMPTY &&
        state->board[to_y][to_x].color == state->board[from_y][from_x].color) {
        return -1; // invalid capture
    }

    return 1; // valid move
}

void move_rook(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    if (is_valid_rook_move(state, from_x, from_y, to_x, to_y) == -1) {
        printf("Invalid move for rook from (%d, %d) to (%d, %d)\n", from_x, from_y, to_x, to_y);
        return; // Mutare invalidă
    }
    Piece temp_piece = state->board[to_y][to_x];
    // Mutare valida
    state->board[to_y][to_x] = state->board[from_y][from_x]; // Muta tura
    state->board[from_y][from_x] = (Piece){EMPTY, WHITE}; // Setează poziția inițială ca fiind goală
    if(is_check(state)){
        // Mutare invalidă, revenire la starea anterioară
        state->board[from_y][from_x] = state->board[to_y][to_x];
        state->board[to_y][to_x] = temp_piece;
        printf("Invalid move! King is in check!\n");
        return;
    }
    state->is_white_turn = !state->is_white_turn; // Schimbă rândul jucătorului
    //printf("Rook moved from (%d, %d) to (%d, %d)\n", from_x, from_y, to_x, to_y);
}