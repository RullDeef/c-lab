#include <curses.h>
#include "game.h"

#ifdef __WIN32
    #include "windows.h"
    #define msleep(ms) Sleep(ms)
#else
    #include "unistd.h"
    #define msleep(ms) usleep(ms * 1000)
#endif

void show_state(struct game_field *gf);

int main(void)
{
    initscr();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    struct game_field gf = game_init(rows, cols);

    game_randomize(&gf, 5);

    while (!game_update(&gf))
    {
        show_state(&gf);
        msleep(200);
        refresh();
    }

    game_destroy(&gf);

    clear();
    printw("Достигнут конец игры.");
    refresh();

    getch();
    endwin();
    return 0;
}

void show_state(struct game_field *gf)
{
    for (int col = 0; col < gf->cols; col++)
        for (int row = 0; row < gf->rows; row++)
            mvprintw(row, col, gf->cells[row][col].alive ? "#" : " ");
}
