#ifndef MOVES_H
#define MOVES_H
#include "board.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "include/bishop.h"
#include "include/king.h"
#include "include/knight.h"
#include "include/pawn.h"
#include "include/queen.h"
#include "include/rook.h"
bool is_valid_move(GameState*, int, int, int, int);
void handle_move(GameState*, int, int, int, int);



#endif
