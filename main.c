#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>

enum FieldValues {
    EMPTY = 0,
    SHIP  = 1 << 0,
    SHOT  = 1 << 1
};

typedef unsigned char cell_t;

typedef struct {
    cell_t **player_a;
    cell_t **player_b;
    size_t length;
} board_t;

cell_t** new_battlefield(int length){
    cell_t **rv = malloc(length*sizeof(cell_t));
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

void draw_cell(int x, int y, cell_t cell){
    char ch;
    switch (cell){
    case EMPTY:
        ch='_';
        break;
    case SHIP | SHOT:
            ch='@';
            break;
    case SHIP:
        ch='#';
        break;
    case SHOT:
        ch='+';
        break;
     default:
        ch='!';
        break;
    }

    mvaddch(y,x, ch);
}

void show(int x, int y, cell_t** field, size_t length){
    int start_x = getcurx(stdscr);
    int start_y = getcury(stdscr);
    move(y, x);

    for(int i=0; i<length; i++){
        for (int j=0; j<length; j++){
            draw_cell(x+i,y+j, (field[i])[j]);
        }
    }
    move(start_y, start_x);
}

int main(void)
{
    initscr();
    noecho();
    keypad(stdscr,TRUE);
    refresh();
    int ch = getch();
    int x=0, y=0;
    board_t *board = new_board(10);

    for (int i=0; i< 10; i++){
        printf("%d\n", board->player_a[i][i]);
        assert(board->player_a[i][i] == 0);
    }

    while (ch != 'q'){
        switch(ch) {
            case KEY_LEFT: x--; break;
            case KEY_RIGHT: x++; break;
            case KEY_UP: y--; break;
            case KEY_DOWN: y++; break;
            default:
            show(10, 10, board->player_a, 10);

        }

        move(y,x);
//        printw("Hello World !!!");
        ch = getch();
        refresh();
    }

    endwin();
    return 0;
}

