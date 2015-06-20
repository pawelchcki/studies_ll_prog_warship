#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>
#include"translation.h"
#include"board.h"
#include"rules.h"
#include"setter.h"

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

void show_board(int y, int x, board_t* board, bool turn_a) {
    show_single(y, x, board->player_a, board->length, turn_a);
    show_single(y, x+ board->length*2 + BOARD_SPACING, board->player_b, board->length, turn_a);
}


bool obeys_limits(cell_t** battlefield, size_t length){
//     jedna „czwórka”, dwie „trójki”, trzy „dwójki”, cztery „jedynki”
    int *counts = count_ships(battlefield, length);
    bool rv = counts[SHIP_FOUR] < 2 && counts[SHIP_THREE] < 3 && counts[SHIP_TWO] < 4 && counts[SHIP_ONE] < 5;
    free(counts);
    return rv;
}

bool final_state(cell_t** battlefield, size_t length){
    int *counts = count_ships(battlefield, length);
    bool rv = counts[SHIP_FOUR] == 1 && counts[SHIP_THREE] == 2 && counts[SHIP_TWO] == 3 && counts[SHIP_ONE] == 4;
    free(counts);
    return rv;
}


cell_t **clone_battlefield(cell_t **battlefield, int board_len){
    cell_t **rv = new_battlefield(board_len);
    for(int i=0; i<board_len; i++)
        for(int j=0; j<board_len; j++){
            rv[i][j] = battlefield[i][j];
        }
    return rv;
}

bool try_obeys(int y, int x, cell_t **battlefield, int board_len){
    cell_t** clone = clone_battlefield(battlefield, board_len);
    start_replace(y,x, clone, board_len);
    bool rv = obeys_limits(clone, board_len);
    free_battlefield(clone, board_len);
    return rv;
}

void place(int y, int x, int y_offset, int x_offset, board_t* board){
    coords_t coords = translate_coords(y,x, y_offset, x_offset, board);
    if (coords.fits){
        cell_t **battlefield;
        if (coords.player_a){
            battlefield = board->player_a;
        } else{
            battlefield = board->player_b;
        }
        if (is_allowed(coords.y, coords.x, battlefield, board->length) && try_obeys(coords.y, coords.x,battlefield, board->length)){
            start_replace(coords.y, coords.x, battlefield, board->length);
        }
    }
}


void dumb_fill(cell_t **battlefield, size_t board_len){
    srand(time(NULL));
    int max_iter= 1000000;
    while(obeys_limits(battlefield, board_len) && max_iter > 0){
//        for(int i=0; i<board_len; i++){
//            for(int j=0; j<board_len; j++){
//                if (rand() % 10 > 2){
//                    if (try_obeys(i, j, battlefield, board_len)){
//                        start_replace(i, j, battlefield, board_len);
//                    }
//                }
//            }
//        }

        int x = rand() % board_len;
        int y = rand() % board_len;
        if (is_allowed(y, x, battlefield, board_len) &&try_obeys(y,x, battlefield, board_len)){
            start_replace(y, x, battlefield, board_len);
        }
        max_iter-=1;
    }

//    while(!final_state(battlefield, board_len))
}

int main(void)
{
//    test_is_alowed();
    initscr();
    noecho();
    keypad(stdscr,TRUE);
    refresh();
    int ch;// = getch();
    int x=0, y=0;
    board_t *board = new_board(10);
    bool turn_a = true;

    while (ch != 'q'){
        switch(ch) {
            case KEY_LEFT: x--; break;
            case KEY_RIGHT: x++; break;
            case KEY_UP: y--; break;
            case KEY_DOWN: y++; break;
            case 'x': remove_at(y,x,0,0, board); break;
        case 't': turn_a = !turn_a ; break;
        case 'r': dumb_fill(board->player_b, board->length);break;
            case 'p': place(y,x,0,0, board); break;
            default:
            show_board(0, 0, board, turn_a);


        }
        show_board(0, 0, board, turn_a);

        move(y,x);
//        printw("Hello World !!!");
        ch = getch();
        refresh();
    }

    endwin();
    return 0;
}

