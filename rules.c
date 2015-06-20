#include"board.h"
#include"rules.h"
#include <assert.h>
#include <string.h>
#include <time.h>

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
