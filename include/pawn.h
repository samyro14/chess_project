#ifndef PAWN_H
#define PAWN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../board.h"
#include "../moves.h"

void move_pawn(GameState*, int, int, int, int);
#endif// PAWN_H