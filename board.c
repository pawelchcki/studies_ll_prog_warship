#include"board.h"

cell_t** new_battlefield(int length){
    cell_t **rv = malloc(length*sizeof(cell_t*));
    assert(rv!=NULL);
    for(int i=0; i<length; i++){
        rv[i] = calloc(length, sizeof(cell_t));
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

void free_board(board_t *board){
    for (int i=0; i<board->length; i++){
        free(board->player_a[i]);
        free(board->player_b[i]);
    }
    free(board->player_a);
    free(board->player_b);
    free(board);
}
