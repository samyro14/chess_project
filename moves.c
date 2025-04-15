#include "moves.h"
#include "board.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// Verificare mutare validă (simplificată)
bool is_valid_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    if (from_x < 0 || from_x >= BOARD_SIZE || from_y < 0 || from_y >= BOARD_SIZE ||
        to_x < 0 || to_x >= BOARD_SIZE || to_y < 0 || to_y >= BOARD_SIZE) {
        return false;
    }
    
    Piece from_piece = state->board[from_y][from_x];
    if (from_piece.type == EMPTY || 
        (from_piece.color == WHITE && !state->is_white_turn) ||
        (from_piece.color == BLACK && state->is_white_turn)) {
        return false;
    }
    
    // Verificare simplă doar pentru pion
    if (from_piece.type == PAWN) {
        int direction = from_piece.color == WHITE ? -1 : 1;
        if (to_x == from_x && to_y == from_y + direction && 
            state->board[to_y][to_x].type == EMPTY) {
            return true;
        }
    }
    
    return true; // Adaugă reguli complete pentru alte piese
}


// Gestionare mutare
void handle_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    if (is_valid_move(state, from_x, from_y, to_x, to_y)) {
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        state->is_white_turn = !state->is_white_turn;
    }
}