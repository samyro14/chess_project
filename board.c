#include "board.h"
#include "moves.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 8
#define SQUARE_SIZE (WINDOW_HEIGHT / BOARD_SIZE)



// Inițializare joc
void init_game(GameState* state) {
    state->window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, 
                                   SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, 
                                   WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
    
    // Inițializare tablă
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            state->board[i][j] = (Piece){EMPTY, WHITE};
        }
    }
    
    // Configurare piese inițiale
    state->board[0][0] = (Piece){ROOK, BLACK};
    state->board[0][1] = (Piece){KNIGHT, BLACK};
    state->board[0][2] = (Piece){BISHOP, BLACK};
    state->board[0][3] = (Piece){QUEEN, BLACK};
    state->board[0][4] = (Piece){KING, BLACK};
    state->board[0][5] = (Piece){BISHOP, BLACK};
    state->board[0][6] = (Piece){KNIGHT, BLACK};
    state->board[0][7] = (Piece){ROOK, BLACK};
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        state->board[1][i] = (Piece){PAWN, BLACK};
        state->board[6][i] = (Piece){PAWN, WHITE};
    }
    
    state->board[7][0] = (Piece){ROOK, WHITE};
    state->board[7][1] = (Piece){KNIGHT, WHITE};
    state->board[7][2] = (Piece){BISHOP, WHITE};
    state->board[7][3] = (Piece){QUEEN, WHITE};
    state->board[7][4] = (Piece){KING, WHITE};
    state->board[7][5] = (Piece){BISHOP, WHITE};
    state->board[7][6] = (Piece){KNIGHT, WHITE};
    state->board[7][7] = (Piece){ROOK, WHITE};
    
    state->is_white_turn = true;
    state->selected_x = -1;
    state->selected_y = -1;
}

// Randare tablă
void render_board(GameState* state) {
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderClear(state->renderer);
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect rect = {j * SQUARE_SIZE, i * SQUARE_SIZE, 
                           SQUARE_SIZE, SQUARE_SIZE};
            
            // Desenare pătrate
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(state->renderer, 240, 217, 181, 255);
            } else {
                SDL_SetRenderDrawColor(state->renderer, 1, 136, 99, 255);
            }
            SDL_RenderFillRect(state->renderer, &rect);
            
            // Desenare selecție
           /* if (i == state->selected_y && j == state->selected_x) {
                SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 100);
                SDL_RenderFillRect(state->renderer, &rect);
            }*/
            
            // Desenare piese (text simplu)
            if (state->board[i][j].type != EMPTY) {
                char piece_char;
                switch (state->board[i][j].type) {
                    case PAWN: piece_char = 'P'; break;
                    case KNIGHT: piece_char = 'N'; break;
                    case BISHOP: piece_char = 'B'; break;
                    case ROOK: piece_char = 'R'; break;
                    case QUEEN: piece_char = 'Q'; break;
                    case KING: piece_char = 'K'; break;
                    default: piece_char = ' ';
                }
                
                SDL_SetRenderDrawColor(state->renderer, 
                                     state->board[i][j].color == WHITE ? 255 : 0, 
                                     0, 
                                     state->board[i][j].color == BLACK ? 255 : 0, 
                                     255);
                SDL_Rect piece_rect = {j * SQUARE_SIZE + 20, i * SQUARE_SIZE + 20, 
                                     40, 40};
                SDL_RenderDrawRect(state->renderer, &piece_rect);
            }
        }
    }
    SDL_RenderPresent(state->renderer);
}

