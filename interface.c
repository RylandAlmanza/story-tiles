#include "interface.h"

int get_color_pair(int fg, int bg) {
    return fg + (bg * 8) + 1;
}

int initialize_all_color_pairs() {
    int fg;
    int bg;
    for (bg = 0; bg < 8; bg++) {
        for (fg = 0; fg < 8; fg++) {
            init_pair(get_color_pair(fg, bg), fg, bg);
        }
    }
}

void interface_init() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    initialize_all_color_pairs();
}

void interface_draw(int x, int y, char chr, int fg, int bg) {
    int attribute = COLOR_PAIR(get_color_pair(fg, bg));
    attron(attribute);
    mvaddch(y, x, chr);
    attroff(attribute);
}

void interface_flush() {
    refresh();
}

void interface_wipe() {
    clear();
}

void interface_uninit() {
    curs_set(1);
    endwin();
}
