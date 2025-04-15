#ifndef MOVES_H
#define MOVES_H
#include "board.h"
#include <stdbool.h>

bool is_valid_move(GameState*, int, int, int, int);
void handle_move(GameState*, int, int, int, int);



#endif
