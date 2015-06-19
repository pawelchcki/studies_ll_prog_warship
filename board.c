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
