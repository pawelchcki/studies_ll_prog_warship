#ifndef BOARD_H
#define BOARD_H
#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>


enum FieldValues {
    EMPTY = 0,
    SHOT  = 1 << 0,
    SHIP = 1 << 1,
    SHIP_ONE = 1 << 2,
    SHIP_TWO = SHIP_ONE + (1 << 3),
    SHIP_THREE = SHIP_TWO + (1 << 4),
    SHIP_FOUR = SHIP_THREE + (1 << 5)
};

#define BOARD_SPACING 2

typedef unsigned char cell_t;

typedef struct {
    cell_t **player_a;
    cell_t **player_b;
    size_t length;
} board_t;

typedef struct {
    int ships[sizeof(char)*255];
} metadata_t;


cell_t** new_battlefield(int length);
board_t* new_board(size_t length);
void free_board(board_t *board);

#endif
