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

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 8
#define SQUARE_SIZE (WINDOW_HEIGHT / BOARD_SIZE)


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