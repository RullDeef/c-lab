#include <stdlib.h>
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
    int status = EXIT_SUCCESS;
    initscr();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    struct game_field gf;


    printw("Enter fill percent (1...100) or 0 to input from file: ");
    int percent;

    if (scanw("%d", &percent) == 1 && (percent == 0 || (percent > 0 && percent <= 100)))
    {
        if (percent == 0)
        {
            char filename[256];
            printw("Enter file name: ");

            if (scanw("%s", &filename) == 1)
            {
                game_load(&gf, filename);
            }
            else
            {
                printw("Wrong filename.");
                status = EXIT_FAILURE;
            }
        }
        else
        {
            gf = game_init(rows, cols);
            game_randomize(&gf, percent);
        }
        
        while (!game_update(&gf))
        {
            show_state(&gf);
            msleep(200);
            refresh();
        }

        clear();
        game_destroy(&gf);
        printw("END OF GAME.");
    }
    else
    {
        printw("Wrong percent.\n");
        status = EXIT_FAILURE;
    }

    refresh();
    getch();
    endwin();
    return status;
}

void show_state(struct game_field *gf)
{
    for (int col = 0; col < gf->cols; col++)
        for (int row = 0; row < gf->rows; row++)
            mvprintw(row, col, gf->cells[row][col].alive ? "#" : " ");
}
