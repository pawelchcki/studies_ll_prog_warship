#include"setter.h"
#include"rules.h"


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
