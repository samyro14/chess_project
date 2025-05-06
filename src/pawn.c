
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../board.h"
#include "../moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

//TO DO! TREBUIE SA VERIFIC DACA ESTE POSIBILA MUTAREA, ASTFEL INCAT SA NU FIE SAH
//TO DO! DIN POZITIA INITIALA DACA PLEACA CU DOUA IN FATA SI ESTE UN PION ADEVERSAR, POATE SA CAPTUREZE
int is_empty(GameState* state, int x, int y){
    if(state->board[y][x].type == EMPTY){
        return 1; // este liber
    }
    return 0; // nu este liber
}

int is_valid_pawn(GameState* state,int from_x, int from_y, int to_x, int to_y){
    int direction = state->board[from_y][from_x].color == WHITE ? -1 : 1;
    // Verificare mutare verticală (în față)
    if (to_x == from_x && to_y == from_y + direction && state->board[to_y][to_x].type == EMPTY) {
        return 1; // Mutare normală
    }
    // Verificare mutare diagonală (capturare)
    if (to_y == from_y + direction) {
        if(to_x == from_x - 1){ // merg in stanga
            if(state->board[to_y][to_x].type != EMPTY && state->board[to_y][to_x].color != state->board[from_y][from_x].color){
                return 2; // Capturare validă
            }
        }
        else if(to_x == from_x + 1){ // merg in dreapta
            if(state->board[to_y][to_x].type != EMPTY && state->board[to_y][to_x].color != state->board[from_y][from_x].color){
                return 3; // Capturare validă
            }
        }
        
    }

    if(from_y == 6 && state->board[from_y][from_x].color == WHITE){ // alb
        if(to_y == from_y - 2 && to_x == from_x && is_empty(state, to_x, to_y) && is_empty(state, to_x, to_y + 1)){ 
            return 4; // mutare dubla
        }
    }
    else if(from_y == 1 && state->board[from_y][from_x].color == BLACK){ // negru
        if(to_y == from_y + 2 && to_x == from_x  && is_empty(state, to_x, to_y) && is_empty(state, to_x, to_y - 1)){
            return 5;
        }
    }

    return -1; // Mutare invalidă
}

void move_pawn(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_pawn(state, from_x, from_y, to_x, to_y) == -1){
        return;
    }
    if(is_valid_pawn(state, from_x, from_y, to_x, to_y) > 0){
        Piece temp_piece = state->board[to_y][to_x];
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        if(is_check(state)){
            // Mutare invalidă, revenire la starea anterioară
            state->board[from_y][from_x] = state->board[to_y][to_x];
            state->board[to_y][to_x] = temp_piece;
            printf("Invalid move! King is in check!\n");
            return;
        }
        state->is_white_turn = !state->is_white_turn;
        return;
    }
}

