#include "moves.h"
#include "board.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Verificare mutare validă
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
    return true;
}


// Gestionare mutare
void handle_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    if (is_valid_move(state, from_x, from_y, to_x, to_y)) {
        // Perform the move
        if (state->board[from_y][from_x].type == PAWN) {
            move_pawn(state, from_x, from_y, to_x, to_y);
        } else if (state->board[from_y][from_x].type == KNIGHT) {
            move_knight(state, from_x, from_y, to_x, to_y);
        } else if (state->board[from_y][from_x].type == BISHOP) {
            move_bishop(state, from_x, from_y, to_x, to_y);
        } else if (state->board[from_y][from_x].type == ROOK) {
            move_rook(state, from_x, from_y, to_x, to_y);
        } else if (state->board[from_y][from_x].type == QUEEN) {
            move_queen(state, from_x, from_y, to_x, to_y);
        } else if (state->board[from_y][from_x].type == KING) {
            move_king(state, from_x, from_y, to_x, to_y);
        }

        // Check for check or checkmate
        if (is_check(state)) {
            if (is_checkmate(state)) {
                printf("Checkmate!\n");
                printf(state->is_white_turn ? "Black wins!\n" : "White wins!\n");
            } else {
                printf("Check!\n");
            }
        }
    }
}