#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>
#include"translation.h"
#include"board.h"

void draw_cell(int y, int x, cell_t cell, bool anonymous){
    char ch;
    if (anonymous && cell == (SHIP | !SHOT)){
        draw_cell(y, x, EMPTY, anonymous);
    }
    switch (cell){
    case EMPTY:
        ch='_';
        break;
    case SHIP | SHOT:
        ch='@';
        break;
    case SHIP_ONE:
        ch='1';
        break;
    case SHIP_TWO:
        ch='2';
        break;
    case SHIP_THREE:
        ch='3';
        break;
    case SHIP_FOUR:
        ch='4';
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

void show_single(int y, int x, cell_t** field, size_t length, bool anonymous){
    int start_x = getcurx(stdscr);
    int start_y = getcury(stdscr);
    move(y, x);

    for(int i=0; i<length; i++){
        for (int j=0; j<length; j++){
            draw_cell(y+j, x+i*2, (field[i])[j], anonymous);
        }
    }
    move(start_y, start_x);
}

void show_board(int y, int x, board_t* board) {
    show_single(y, x, board->player_a, board->length, true);
    show_single(y, x+ board->length*2 + BOARD_SPACING, board->player_b, board->length, true);
}


void place(int y, int x, int y_offset, int x_offset, board_t* board){
    if (fits){
        if (player_a){
            board->player_a[translated_x][translated_y] = SHIP_ONE;
//            mvaddch(translated_y, translated_x*2, 'D');
        } else{
            board->player_b[translated_x][translated_y] = SHIP_ONE;
//            mvaddch(translated_y, translated_x*2 + board->length*2 + BOARD_SPACING, 'D');
        }
    }
}

int main(void)
{
    initscr();
    noecho();
    keypad(stdscr,TRUE);
    refresh();
    int ch;// = getch();
    int x=0, y=0;
    board_t *board = new_board(10);

    while (ch != 'q'){
        switch(ch) {
            case KEY_LEFT: x--; break;
            case KEY_RIGHT: x++; break;
            case KEY_UP: y--; break;
            case KEY_DOWN: y++; break;
        case 'p': place(y,x,0,0, board);
            default:
            show_board(0, 0, board);

        }

        move(y,x);
//        printw("Hello World !!!");
        ch = getch();
        refresh();
    }

    endwin();
    return 0;
}

