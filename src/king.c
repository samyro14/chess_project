#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../board.h"
#include "../moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_king_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
  
}
int is_check(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    // Check if the move puts the king in check
    // This is a simplified version and should be expanded for full functionality
    return 0; // 0 = not in check, 1 = in check
}
int is_checkmate(GameState* state) {
    // Check if the king is in checkmate
    // This is a simplified version and should be expanded for full functionality
    return 0; // 0 = not in checkmate, 1 = in checkmate
}
void move_king(GameState* state, int from_x, int from_y, int to_x, int to_y){
    
}