#include"board.h"
#include <string.h>

cell_t** new_battlefield(int length){
    cell_t **rv = malloc(length*sizeof(cell_t*));
    assert(rv != NULL);

    for(int i=0; i<length; i++){
        rv[i] = calloc(length, sizeof(cell_t));
        assert(rv[i] != NULL);
    }
    return rv;
}

board_t* new_board(size_t length) {
    board_t *board = malloc(sizeof(board_t));
    board->player_a = new_battlefield(length);
    board->player_b = new_battlefield(length);
    board->length = length;
    return board;
}

void free_battlefield(cell_t **battlefield, size_t length) {
    for (int i=0; i< length; i++){
        free(battlefield[i]);
    }
    free(battlefield);
}

void free_board(board_t *board){
    free_battlefield(board->player_a, board->length);
    free_battlefield(board->player_b, board->length);
    free(board);
}

cell_t** battlefield_from_matrix(void* matrix, size_t length){
    cell_t (*filled)[length] = matrix;
    cell_t** rv = new_battlefield(length);
    for(int i=0; i<length; i++){
        for(int j=0; j< length; j++){
            cell_t x = filled[i][j];
            rv[i][j] = x;
        }

//        memcpy(rv[i],filled[i], length);
    }
    return rv;
}

void get_direction(int sy, int sx, int *y_dir, int *x_dir, cell_t **player, int board_len){
    int dirs[4][2] = {{-1,0}, {0,-1}, {1, 0},{0,1}};
    for(int i=0; i < 4; i++){
        *y_dir = dirs[i][0];
        *x_dir = dirs[i][1];
        int x = sx + *x_dir;
        int y = sy + *y_dir;
        if (x >= 0 && y >= 0 && x < board_len && y < board_len){
            if ((player[x][y] & SHIP) == SHIP){
                return;
            }
        }
    }
    *y_dir = 0;
    *x_dir = 0;
}

int* count_ships(cell_t** battlefield, size_t length){
    int* counts;
    counts  = malloc(255 * sizeof(*counts));
    memset(counts, 0, 255);
    for(int i = 0; i< length; i++ ){
        for(int j = 0; j< length; j++){
            cell_t ship = battlefield[i][j];
            if (ship == SHIP_FOUR){
                counts[SHIP_FOUR] += 1;
            } else if (ship  == SHIP_THREE){
                counts[SHIP_THREE] += 1;
            } else if (ship == SHIP_TWO){
                counts[SHIP_TWO] += 1;
            } else if (ship == SHIP_ONE){
                counts[SHIP_ONE] += 1;
            }
        }
    }
    counts[SHIP_FOUR] /= 4;
    counts[SHIP_THREE] /= 3;
    counts[SHIP_TWO] /= 2;
    return counts;
}
