#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../src/board.h"
#include "../src/moves.h"
#include <stdio.h>
#define BOARD_SIZE 8

int is_valid_king_move(GameState* state, int from_x, int from_y, int to_x, int to_y) {
    int dy[] = {-1, -1, -1, 0, 1, 1, 1, 0},
        dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
    for(int i = 0; i < 8; i++){
        if(to_x == from_x + dx[i] && to_y == from_y + dy[i] && (state->board[to_y][to_x].type == EMPTY  || state->board[to_y][to_x].color != state->board[from_y][from_x].color)){
            return 1; // Valid move
        }
    }
    return -1; // Invalid move
}
int get_coordinates_king(GameState* state, int* x_king, int* y_king){
    PieceColor turn_color = BLACK;
    if(state->is_white_turn)
        turn_color = WHITE;
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(state->board[i][j].type == KING && state->board[i][j].color == turn_color){
                (*x_king) = j;
                (*y_king) = i;
                return 1; // Found the king
            }
        }
    }
    
    return -1; // King not found

}
int is_check(GameState* state) {
    PieceColor turn_color = BLACK;
    if(state->is_white_turn)
        turn_color = WHITE;
    int y_king = 0, x_king = 0;
    if(get_coordinates_king(state, &x_king, &y_king) == -1){
        printf("King not found!\n");
        return -1; // King not found
    }
    // Check if the king is in check
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(state->board[i][j].color != turn_color && state->board[i][j].type != EMPTY){
                switch (state->board[i][j].type) {
                case PAWN:
                    if(is_valid_pawn(state, j, i, x_king, y_king) == 2 || is_valid_pawn(state, j, i, x_king, y_king) == 3)
                        return 1; // In check
                    break;
                case KNIGHT:
                    if(is_valid_knight_move(state, j, i, x_king, y_king) == 1)
                        return 1; // In check
                    break;
                case BISHOP:
                    if(is_valid_bishop_move(state, j, i, x_king, y_king) == 1)
                        return 1; // In check
                    break;
                case ROOK:  
                    if(is_valid_rook_move(state, j, i, x_king, y_king) == 1)
                        return 1; // In check
                    break;
                case QUEEN: 
                    if(is_valid_queen_move(state, j, i, x_king, y_king) == 1)
                        return 1; // In check
                    break;
                case KING:
                    if(is_valid_king_move(state, j, i, x_king, y_king) == 1)
                        return 1; // In check
                    break;
                default:
                    break;
                }
            }
        }
    }
    return 0; // 0 = not in check, 1 = in check
}
int is_checkmate(GameState* state) {
    // Check if the king is in checkmate
    PieceColor turn_color = BLACK;
    if(state->is_white_turn)
        turn_color = WHITE;
    
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(state->board[i][j].color == turn_color && state->board[i][j].type != EMPTY){
                for(int y = 0; y < 8; ++y){
                    for(int x = 0; x < 8; ++x){
                        if(is_valid_move(state, j, i, x, y)){
                            Piece temp_piece = state->board[y][x];
                            // Move the piece
                            // state->board[y][x] = state->board[i][j];
                            // state->board[i][j] = (Piece){EMPTY, WHITE};
                            switch (state->board[i][j].type) {
                                case PAWN:
                                    if(is_valid_pawn(state, j, i, x, y) > 0){
                                        state->board[y][x] = state->board[i][j];
                                        state->board[i][j] = (Piece){EMPTY, WHITE};
                                        if(is_check(state) == 0){
                                            // Undo the move if it doesn't put the king in check
                                            state->board[i][j] = state->board[y][x];
                                            state->board[y][x] = temp_piece;
                                            return 0; // Not in checkmate
                                        }
                                        // Undo the move
                                        state->board[i][j] = state->board[y][x];
                                        state->board[y][x] = temp_piece;
                                    }
                                    break;
                                case KNIGHT:
                                    if(is_valid_knight_move(state, j, i, x, y) == 1){
                                        state->board[y][x] = state->board[i][j];
                                        state->board[i][j] = (Piece){EMPTY, WHITE};
                                        if(is_check(state) == 0){
                                            // Undo the move if it doesn't put the king in check
                                            state->board[i][j] = state->board[y][x];
                                            state->board[y][x] = temp_piece;
                                            return 0; // Not in checkmate
                                        }
                                        state->board[i][j] = state->board[y][x];
                                        state->board[y][x] = temp_piece;
                                    }
                                    break;
                                case BISHOP:
                                    if(is_valid_bishop_move(state, j, i, x, y) == 1){
                                        state->board[y][x] = state->board[i][j];
                                        state->board[i][j] = (Piece){EMPTY, WHITE};
                                        if(is_check(state) == 0){
                                            // Undo the move if it doesn't put the king in check
                                            state->board[i][j] = state->board[y][x];
                                            state->board[y][x] = temp_piece;
                                            return 0; // Not in checkmate
                                        }
                                        state->board[i][j] = state->board[y][x];
                                        state->board[y][x] = temp_piece;
                                    }
                                    break;
                                case ROOK:  
                                    if(is_valid_rook_move(state, j, i, x, y) == 1){
                                        state->board[y][x] = state->board[i][j];
                                        state->board[i][j] = (Piece){EMPTY, WHITE};
                                        if(is_check(state) == 0){
                                            // Undo the move if it doesn't put the king in check
                                            state->board[i][j] = state->board[y][x];
                                            state->board[y][x] = temp_piece;
                                            return 0; // Not in checkmate
                                        }
                                        state->board[i][j] = state->board[y][x];
                                        state->board[y][x] = temp_piece;
                                    }
                                    break;
                                case QUEEN: 
                                    if(is_valid_queen_move(state, j, i, x, y) == 1){
                                        state->board[y][x] = state->board[i][j];
                                        state->board[i][j] = (Piece){EMPTY, WHITE};
                                        if(is_check(state) == 0){
                                            // Undo the move if it doesn't put the king in check
                                            state->board[i][j] = state->board[y][x];
                                            state->board[y][x] = temp_piece;
                                            return 0; // Not in checkmate
                                        }
                                        state->board[i][j] = state->board[y][x];
                                        state->board[y][x] = temp_piece;
                                    }
                                    break;
                                case KING:
                                    if(is_valid_king_move(state, j, i, x, y) == 1){
                                        state->board[y][x] = state->board[i][j];
                                        state->board[i][j] = (Piece){EMPTY, WHITE};
                                        if(is_check(state) == 0){
                                            // Undo the move if it doesn't put the king in check
                                            state->board[i][j] = state->board[y][x];
                                            state->board[y][x] = temp_piece;
                                            return 0; // Not in checkmate
                                        }
                                        state->board[i][j] = state->board[y][x];
                                        state->board[y][x] = temp_piece;
                                    }
                                    break;
                                default:
                                    break;
                            }        
                        }
                    }
                }
            }
        }
    }
    return 1; // 0 = not in checkmate, 1 = in checkmate
}
void move_king(GameState* state, int from_x, int from_y, int to_x, int to_y){
    if(is_valid_king_move(state, from_x, from_y, to_x, to_y) == 1){
        Piece temp_piece = state->board[to_y][to_x];
        // Move the king
        state->board[to_y][to_x] = state->board[from_y][from_x];
        state->board[from_y][from_x] = (Piece){EMPTY, WHITE};
        if(is_check(state)){
            // Undo the move if it puts the king in check
            state->board[from_y][from_x] = state->board[to_y][to_x];
            state->board[to_y][to_x] = temp_piece;
            printf("Invalid move! King is in check!\n");
            return;
        }
        state->is_white_turn = !state->is_white_turn;
        return;
        
    } else {
        printf("Invalid move for King\n");
    }
}