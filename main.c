#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "board.h"
#include "moves.h"

#define WINDOW_WIDTH 800
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
            if (i == state->selected_y && j == state->selected_x) {
                SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 100);
                SDL_RenderFillRect(state->renderer, &rect);
            }
            
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

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    GameState state;
    init_game(&state);
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x / SQUARE_SIZE;
                int y = event.button.y / SQUARE_SIZE;
                
                if (state.selected_x == -1) {
                    if (state.board[y][x].type != EMPTY) {
                        state.selected_x = x;
                        state.selected_y = y;
                    }
                } else {
                    handle_move(&state, state.selected_x, state.selected_y, x, y);
                    state.selected_x = -1;
                    state.selected_y = -1;
                }
            }
        }
        
        render_board(&state);
    }
    
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
    
    return 0;
}