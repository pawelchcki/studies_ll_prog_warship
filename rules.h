#ifndef RULES_H
#define RULES_H
#include "board.h"
#include"translation.h"

bool check_valid_ship(int y, int x, int y_dir, int x_dir, int *ship_len, cell_t **player, int board_len);
bool is_allowed(int y, int x, cell_t **player, int board_len);
bool check_neighbor(int y, int x, int cy, int cx, cell_t **player, int board_len);

#endif
