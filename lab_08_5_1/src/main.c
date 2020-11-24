#include "matrix/matrix.h"

int main(int argc, const char **argv)
{
    int status = 0;

    struct matrix A = mat_read(stdin);

    if (mat_is_valid(A))
    {
        struct matrix B = mat_read(stdin);

        if (mat_is_valid(B))
        {
            // MAIN TASK

            A = mat_make_square(A);
            B = mat_make_square(B);

            int k = A.rows;
            int s = B.rows;
            int z = k > s ? k : s;

            if (z > A.rows)
                A = mat_expand(A, z);
            else if (z > B.rows)
                B = mat_expand(B, z);

            int p, q;
            if (scanf("%d%d", &p, &q) != 2 || p < 0 || q < 0)
                status = -3;
            else
            {
                A = mat_pow(A, p);
                B = mat_pow(B, q);
                struct matrix C = mat_mult(A, B);

                if (mat_is_valid(C))
                {
                    mat_print(stdout, C);
                    mat_destroy(C);
                }
                else
                    status = -4;
            }
            // END MAIN TASK

            mat_destroy(B);
        }
        else
            status = -2;

        mat_destroy(A);
    }
    else
        status = -1;

    return status;
}
