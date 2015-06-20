#ifndef SETTER
#define SETTER
#include "board.h"
#include"translation.h"

void replace(int y, int x, int y_dir, int x_dir, cell_t **player, int board_len, cell_t ship);
cell_t len_to_ship(int len);
void start_replace(int y, int x, cell_t **player, int board_len);
void remove_at(int y, int x, int y_offset, int x_offset, board_t* board);

#endif
