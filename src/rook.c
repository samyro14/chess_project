#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../board.h"
#include "../moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_rook(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    // Verificare coordonate valide
    if (from_x < 0 || from_x >= BOARD_SIZE || from_y < 0 || from_y >= BOARD_SIZE ||
        to_x < 0 || to_x >= BOARD_SIZE || to_y < 0 || to_y >= BOARD_SIZE) {
        return -1; // Mutare invalidă
    }
    if(state->board[from_y][from_x].type != ROOK) {
        return -1; // Piesa selectată nu este o tură
    }


    // Verificare mutare orizontală sau verticală
    if (from_x != to_x && from_y != to_y) {
        return -1; // Mutare invalidă
    }

    // Verificare dacă drumul este liber
    if (from_x == to_x) { // Mutare pe verticală
        int step = (to_y > from_y) ? 1 : -1;
        for (int i = from_y + step; i != to_y; i += step) {
            if (state->board[i][from_x].type != EMPTY) {
                return -1; // Drumul nu este liber
            }
        }
    } else if (from_y == to_y) { // Mutare pe orizontală
        int step = (to_x > from_x) ? 1 : -1;
        for (int i = from_x + step; i != to_x; i += step) {
            if (state->board[from_y][i].type != EMPTY) {
                return -1; // Drumul nu este liber
            }
        }
    }

    // Verificare dacă destinația este ocupată de o piesă de aceeași culoare
    if (state->board[to_y][to_x].type != EMPTY &&
        state->board[to_y][to_x].color == state->board[from_y][from_x].color) {
        return -1; // Destinația este ocupată de o piesă de aceeași culoare
    }

    return 1; // Mutare validă
}

void move_rook(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    if (is_valid_rook(state, from_x, from_y, to_x, to_y) == -1) {
        printf("Invalid move for rook from (%d, %d) to (%d, %d)\n", from_x, from_y, to_x, to_y);
        return; // Mutare invalidă
    }

    // Mutare valida
    state->board[to_y][to_x] = state->board[from_y][from_x]; // Muta tura
    state->board[from_y][from_x] = (Piece){EMPTY, WHITE}; // Setează poziția inițială ca fiind goală
    state->is_white_turn = !state->is_white_turn; // Schimbă rândul jucătorului
    //printf("Rook moved from (%d, %d) to (%d, %d)\n", from_x, from_y, to_x, to_y);
}