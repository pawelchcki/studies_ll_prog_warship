#include <stdio.h>
#include<ncurses.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>

#include"translation.h"
#include"translation.h"

coords_t translate_coords(int y, int x, int y_offset, int x_offset, board_t* board){
    bool player_a = false;
    bool fits = false;
    int translated_x = x - x_offset;

    if (translated_x < board->length*2){
        player_a = true;
        if (translated_x % 2 == 0){
            fits = true;
        }
        translated_x /= 2;
    } else {
        translated_x -= BOARD_SPACING + board->length*2;
        if (translated_x < board->length*2){
            if (translated_x % 2 == 0){
                fits = true;
            }
            translated_x /= 2;
        }
    }

    int translated_y = y - y_offset;
    coords_t rv = { .x = translated_x % board->length,
                    .y = translated_y % board->length,
                    .fits = fits,
                    .player_a = player_a,
                    .x_origin = x,
                    .y_origin = y,
                    .x_offset = x_offset,
                    .y_offset = y_offset
                   };
    return rv;
//    fprintf(stderr, "%d %d %d %d\n", translated_x, translated_y, translated_x, player_a);
}
