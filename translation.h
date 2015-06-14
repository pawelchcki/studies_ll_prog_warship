#ifndef TRANSLATION_H
#define TRANSLATION_H
#include"board.h"
#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>

typedef struct {
    int x_origin, y_origin;
    int x_offset, y_offset;
    int x, y;
    bool fits;
    bool player_a;
} coords_t;


coords_t translate_coords(int y, int x, int y_offset, int x_offset, board_t* board);
#endif
