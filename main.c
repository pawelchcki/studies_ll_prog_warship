#include <stdio.h>
#include<ncurses.h>

int main(void)
{
    initscr();
    noecho();
    keypad(stdscr,TRUE);
    refresh();
    int ch = getch();
    int x=0, y=0;
    while (ch != 'q'){
        switch(ch) {
            case KEY_LEFT: y--; break;
            case KEY_RIGHT: y++; break;
            case KEY_UP: x--; break;
            case KEY_DOWN: x++; break;
            default: printw("Hello");
        }
        move(x,y);
//        printw("Hello World !!!");
        ch = getch();
        refresh();
    }

    endwin();
    return 0;
}

