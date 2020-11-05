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

int input(struct game_field *gf);
void mainloop(struct game_field *gf);
void show_state(struct game_field *gf);


int main(void)
{
    struct game_field gf;

    initscr();
    int status = input(&gf);

    if (status == EXIT_SUCCESS)
    {
        mainloop(&gf);

        clear();
        game_destroy(&gf);
        printw("END OF GAME.");
    }

    refresh();
    getch();
    endwin();
    return status;
}

int input(struct game_field *gf)
{
    int status = EXIT_SUCCESS;
    int percent;

    int rows, cols; // std screen size
    getmaxyx(stdscr, rows, cols);

    printw("Enter fill percent (1...100) or 0 to input from file: ");

    if (scanw("%d", &percent) == 1 && (percent == 0 || (percent > 0 && percent <= 100)))
    {
        if (percent == 0)
        {
            char filename[256];
            printw("Enter file name: ");

            if (scanw("%s", &filename) == 1)
            {
                if (game_load(gf, filename) != EXIT_SUCCESS)
                {
                    printw("invalid file.");
                    status = EXIT_FAILURE;
                }
            }
            else
            {
                printw("Wrong filename.");
                status = EXIT_FAILURE;
            }
        }
        else
        {
            *gf = game_init(rows, cols);
            game_randomize(gf, percent);
        }
    }
    else
    {
        printw("Wrong percent.\n");
        status = EXIT_FAILURE;
    }

    return status;
}

void mainloop(struct game_field *gf)
{
    do
    {
        show_state(gf);
        msleep(200);
        refresh();
    }
    while (!game_update(gf));
}

void show_state(struct game_field *gf)
{
    for (int col = 0; col < gf->cols; col++)
        for (int row = 0; row < gf->rows; row++)
            mvprintw(row, col, gf->cells[row][col].alive ? "#" : " ");
}
