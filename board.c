#include "board.h"
#include "moves.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

//Randare piesă
void render_piece(GameState* state, const char* piece, int x, int y) {
    SDL_Surface* surface = IMG_Load(piece);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(state->renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect dest_rect = {x * SQUARE_SIZE + 10, y * SQUARE_SIZE + 10, 
                        SQUARE_SIZE - 20, SQUARE_SIZE - 20};
    SDL_RenderCopy(state->renderer, texture, NULL, &dest_rect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Randare tablă
void render_board(GameState* state) {
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderClear(state->renderer);
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        return ;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect rect = {j * SQUARE_SIZE, i * SQUARE_SIZE, 
                           SQUARE_SIZE, SQUARE_SIZE};
            
            // Desenare pătrate
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(state->renderer, 235,236,208,255);//alb
            } else {
                SDL_SetRenderDrawColor(state->renderer, 115,149,82,255); //negru
            }
            SDL_RenderFillRect(state->renderer, &rect);
            
            // Desenare selecție
            if (i == state->selected_y && j == state->selected_x) {
                SDL_SetRenderDrawColor(state->renderer, 101, 101, 101, 100);
                SDL_RenderFillRect(state->renderer, &rect);
            }
          
            // Desenare piese (text simplu)
            if (state->board[i][j].type != EMPTY) {
                char piece[100] = ""; // Initializează un buffer pentru piesă
                if(state->board[i][j].color == WHITE){
                    switch (state->board[i][j].type) {
                        case PAWN:strncpy(piece, "assets/img/pieces/white/pawn.png", 100); break;
                        case KNIGHT: strncpy(piece, "assets/img/pieces/white/knight.png", 100); break;
                        case BISHOP: strncpy(piece, "assets/img/pieces/white/bishop.png", 100); break;
                        case ROOK: strncpy(piece, "assets/img/pieces/white/rook.png", 100); break;
                        case QUEEN: strncpy(piece, "assets/img/pieces/white/queen.png", 100); break;
                        case KING: strncpy(piece, "assets/img/pieces/white/king.png", 100); break;
                        default: break;
                }
                }
                else{
                    switch (state->board[i][j].type) {
                        case PAWN: strncpy(piece, "assets/img/pieces/black/pawn.png", 100); break;
                        case KNIGHT: strncpy(piece, "assets/img/pieces/black/knight.png", 100); break;
                        case BISHOP: strncpy(piece, "assets/img/pieces/black/bishop.png", 100); break;
                        case ROOK: strncpy(piece, "assets/img/pieces/black/rook.png", 100); break;
                        case QUEEN: strncpy(piece, "assets/img/pieces/black/queen.png", 100); break;
                        case KING: strncpy(piece, "assets/img/pieces/black/king.png", 100); break;
                        default: break;
                    }
                }
                render_piece(state, piece, j, i);
            }
        }
    }
    SDL_RenderPresent(state->renderer);
}

