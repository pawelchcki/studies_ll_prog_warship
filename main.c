#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>
#include"translation.h"
#include"board.h"
#include <assert.h>
#include <string.h>
#include <time.h>

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


// 3b 20 czerwca sobota
bool check_neighbor(int y, int x, coords_t *c, cell_t **player, int board_len){
    int x_n = c->x + x;
    int y_n = c->y + y;
    if (x_n < 0 || y_n < 0 || x_n >= board_len || y_n >= board_len){
        return false;
    }

    if (player[x_n][y_n] & SHIP){
        return true;
    } else {
        return false;
    }
}

bool check_valid_ship(int y, int x, int y_dir, int x_dir, int *ship_len, cell_t **player, int board_len){
    if (*ship_len > 4) {
        return false;
    }

    if (x >= 0 && y >= 0 && x < board_len && y < board_len){
        if (player[x][y] & SHIP){
            *ship_len += 1;
            if (y_dir == 0 && x_dir == 0) {
                return true;
            } else {
                return check_valid_ship(y + y_dir, x + x_dir, y_dir, x_dir, ship_len, player, board_len);
            }
        }
    }
    return true;
}

bool is_allowed(coords_t *c, cell_t **player, int board_len){
    int neighbors=0;
    //niedozwoleni
    if (check_neighbor(-1, -1, c, player, board_len) || check_neighbor(1, 1, c, player, board_len) || check_neighbor(-1, 1, c, player, board_len) || check_neighbor(1, -1, c, player, board_len)){
        return false;
    }
    int x_last_neigh, y_last_neigh;
    int allowed_neighboars[4][2] = {{-1,0}, {0,-1}, {1, 0},{0,1}};
    for(int i=0; i < 4; i++){
        if (check_neighbor(allowed_neighboars[i][0], allowed_neighboars[i][1], c, player, board_len)){
            neighbors += 1;
            y_last_neigh = allowed_neighboars[i][0];
            x_last_neigh = allowed_neighboars[i][1];
        }
    }

    if (neighbors > 1){
        return false;
    } else if (neighbors == 1){
        int ship_len = 1;
        return check_valid_ship(c->y + y_last_neigh, c->x + x_last_neigh, y_last_neigh, x_last_neigh, &ship_len, player, board_len);

    }
    return true;
}



void test_is_alowed(){
    coords_t c;
    c.fits = true;
    c.player_a = true;
    c.x = 0;
    c.y = 1;
    {
        int size = 5;
        BATTLEFIELD(max_len, ARR({{EMPTY,SHIP,SHIP,SHIP,SHIP},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}), 5)
        c.x = 0;
        c.y = 0;
        assert(is_allowed(&c, max_len_bf, size) == false);
        free_battlefield(max_len_bf, size);
    }
    {
        int size = 2;
        BATTLEFIELD(simple, ARR({{SHIP,EMPTY}, {EMPTY,EMPTY}}), 2)
        assert(is_allowed(&c, simple_bf, size) == true);
        c.x=1;
        c.y=0;
        assert(is_allowed(&c, simple_bf, size) == true);
        simple_bf[0][1] |= SHIP;
        assert(is_allowed(&c, simple_bf, size) == false);
        free_battlefield(simple_bf, size);
    }

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


void replace(int y, int x, int y_dir, int x_dir, cell_t **player, int board_len, cell_t ship){
    if ((y < board_len && x < board_len && y >=0 && x >= 0) && (player[x][y] & SHIP)==SHIP){
      player[x][y] = ship;
      if (y_dir != 0 || x_dir != 0){
        replace(y+y_dir, x+x_dir, y_dir, x_dir, player, board_len, ship);
      }
    }
}

cell_t len_to_ship(int len){
    switch(len){
    case 1: return SHIP_ONE;
    case 2: return SHIP_TWO;
    case 3: return SHIP_THREE;
    case 4: return SHIP_FOUR;
    }
    return SHIP;
}


void start_replace(int y, int x, cell_t **player, int board_len){
    player[x][y] |= SHIP;
    int ship_len = 0;
    int x_dir, y_dir;
    get_direction(y, x, &y_dir, &x_dir, player, board_len);
    if (check_valid_ship(y, x, y_dir, x_dir, &ship_len, player, board_len)){
        replace(y, x, y_dir, x_dir, player, board_len, len_to_ship(ship_len));
    } else {
        player[x][y] ^= SHIP;
    }
}


void remove_at(int y, int x, int y_offset, int x_offset, board_t* board){
    coords_t coords = translate_coords(y,x, y_offset, x_offset, board);
    if (coords.fits){
        cell_t **battlefield;
        if (coords.player_a){
            battlefield = board->player_a;
        } else{
            battlefield = board->player_b;
        }
        if (battlefield[coords.x][coords.y] & SHIP){
            int y_dir = 0, x_dir =0;
            battlefield[coords.x][coords.y] = EMPTY;
            for(int i=0; i<4; i++){
                get_direction(coords.y, coords.x, &y_dir, &x_dir, battlefield, board->length);
                if (y_dir == 0 && x_dir == 0){
                    break;
                } else {
                    replace(coords.y+y_dir, coords.x+x_dir, y_dir, x_dir, battlefield, board->length, EMPTY);
                }
            }
        }
    }
}

int* count_ships(cell_t** battlefield, size_t length){
    int* counts;
    counts  = malloc(255 * sizeof(*counts));
    memset(counts, 0, 255);
    for(int i = 0; i< length; i++ ){
        for(int j = 0; j< length; j++){
            cell_t ship = battlefield[i][j];
            if (ship & SHIP_FOUR){
                counts[SHIP_FOUR] += 1;
            } else if (ship & SHIP_THREE){
                counts[SHIP_THREE] += 1;
            } else if (ship & SHIP_TWO){
                counts[SHIP_TWO] += 1;
            } else if (ship & SHIP_ONE){
                counts[SHIP_ONE] += 1;
            }
        }
    }
    counts[SHIP_FOUR] /= 4;
    counts[SHIP_THREE] /= 3;
    counts[SHIP_TWO] /= 2;
    return counts;
}

bool obeys_limits(cell_t** battlefield, size_t length){
//     jedna „czwórka”, dwie „trójki”, trzy „dwójki”, cztery „jedynki”
    int *counts = count_ships(battlefield, length);
    bool rv = counts[SHIP_FOUR] < 2 && counts[SHIP_THREE] < 3 && counts[SHIP_TWO] < 4 && counts[SHIP_ONE] < 1;
    free(counts);
    return rv;
}

bool final_state(cell_t** battlefield, size_t length){
    int *counts = count_ships(battlefield, length);
    bool rv = counts[SHIP_FOUR] == 1 && counts[SHIP_THREE] == 2 && counts[SHIP_TWO] == 3 && counts[SHIP_ONE] == 4;
    free(counts);
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
        if (is_allowed(&coords, battlefield, board->length)){
            start_replace(coords.y, coords.x, battlefield, board->length);
        }
    }
}


void dumb_fill(cell_t **battlefield, size_t board_len){
    srand(time(NULL));
    while(!final_state(battlefield, board_len)){
        int x = rand() % board_len;
        int y = rand() % board_len;
    }
}

int main(void)
{
    test_is_alowed();
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
        case 'x': remove_at(y,x,0,0, board); break;
        case 'p': place(y,x,0,0, board); break;
            default:
            show_board(0, 0, board);

        }
        show_board(0, 0, board);

        move(y,x);
//        printw("Hello World !!!");
        ch = getch();
        refresh();
    }

    endwin();
    return 0;
}

