#include "matrix/matrix.h"

int main(int argc, const char **argv)
{
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
