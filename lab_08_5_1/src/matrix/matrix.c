#include "matrix.h"
#include <math.h>
#include <stdlib.h>

struct matrix mat_create(int rows, int cols)
{
    struct matrix mat = { .rows = rows, .cols = cols };
    mat.data = calloc(rows, sizeof(int*));

    if (mat.data != NULL)
    {
        bool mem_error = false;
        for (int row = 0; row < rows && !mem_error; row++)
        {
            mat.data[row] = calloc(cols, sizeof(int));
            mem_error = mat.data[row] == NULL;
        }

        if (mem_error)
        {
            mat_destroy(mat);
            mat = mat_invalid();
        }
    }

    return mat;
}

struct matrix mat_invalid(void)
{
    struct matrix mat = { .data = NULL };
    return mat;
}

void mat_destroy(struct matrix mat)
{
    for (int row = 0; row < mat.rows; row++)
        free(mat.data[row]);
    free(mat.data);
}

bool mat_is_valid(struct matrix mat)
{
    return mat.data != NULL;
}

struct matrix mat_read(FILE *file)
{
    int rows, cols;

    if (fscanf(file, "%d%d", &rows, &cols) != 2 || rows <= 0 || cols <= 0)
        return mat_invalid();

    struct matrix mat = mat_create(rows, cols);

    bool input_valid = true;
    for (int row = 0; row < rows && input_valid; row++)
        for (int col = 0; col < cols && input_valid; col++)
            input_valid = fscanf(file, "%d", mat.data[row] + col) == 1;
    
    if (!input_valid)
    {
        mat_destroy(mat);
        mat = mat_invalid();
    }

    return mat;
}

int mat_print(FILE *file, struct matrix mat)
{
    bool output_valid = true;

    for (int row = 0; row < mat.rows && output_valid; row++)
    {
        for (int col = 0; col < mat.cols && output_valid; col++)
            output_valid = fprintf(file, " %d" + (col == 0), mat.data[row][col]) > 0;
        fprintf(file, "\n");
    }

    return output_valid ? 1 : 0;
}

static void get_min_elem_index(struct matrix mat, int *min_row, int *min_col)
{
    int min = mat.data[0][0];
    *min_row = 0;
    *min_col = 0;

    for (int row = 0; row < mat.rows; row++)
    {
        for (int col = 0; col < mat.cols; col++)
        {
            if (mat.data[row][col] < min)
            {
                min = mat.data[row][col];
                *min_row = row;
                *min_col = col;
            }
        }
    }
}

struct matrix mat_make_square(struct matrix mat)
{
    int min_row, min_col;

    while (mat.rows > mat.cols)
    {
        get_min_elem_index(mat, &min_row, &min_col);
        free(mat.data[min_row]);

        for (int row = min_row; row + 1 < min_row; row++)
            mat.data[row] = mat.data[row + 1];

        mat.rows--;
    }

    while (mat.cols > mat.rows)
    {
        get_min_elem_index(mat, &min_row, &min_col);

        for (int row = 0; row < mat.rows; row++)
            for (int col = min_col; col + 1 < mat.cols; col++)
                mat.data[row][col] = mat.data[row][col + 1];

        mat.cols--;
    }

    return mat;
}

static int get_geom_mean(struct matrix mat, int col)
{
    float mean = 1.0f;

    for (int row = 0; row < mat.rows; row++)
        mean *= mat.data[row][col];

    return (int)floorf(powf(mean, 1.0f / mat.rows));
}

static int get_max(struct matrix mat, int row)
{
    int max = mat.data[0][0];

    for (int col = 0; col < mat.cols; col++)
        max = mat.data[row][col] > max ? mat.data[row][col] : max;
    
    return max;
}

struct matrix mat_expand(struct matrix mat, int z)
{
    int **new_data = realloc(mat.data, z * sizeof(int*));
    if (new_data == NULL)
    {
        mat_destroy(mat);
        return mat_invalid();
    }
    else
    {
        mat.data = new_data;

        bool mem_error = false;
        for (int row = 0; row < mat.rows && !mem_error; row++)
        {
            int *new_row = realloc(mat.data[row], z * sizeof(int));
            mem_error = new_row == NULL;
            if (!mem_error)
                mat.data[row] = new_row;
        }

        if (mem_error)
        {
            mat_destroy(mat);
            return mat_invalid();
        }

        for (int row = mat.rows; row < z; row++)
            mat.data[row] = calloc(z, sizeof(int));
    }

    while (mat.rows < z)
    {
        for (int col = 0; col < mat.cols; col++)
            mat.data[mat.rows][col] = get_geom_mean(mat, col);

        mat.rows++;
    }

    while (mat.cols < z)
    {
        for (int row = 0; row < mat.rows; row++)
            mat.data[row][mat.cols] = get_max(mat, row);

        mat.cols++;
    }

    return mat;
}

struct matrix mat_mult(struct matrix left, struct matrix right)
{
    struct matrix res = mat_create(left.rows, right.cols);

    if (mat_is_valid(res))
    {
        for (int row = 0; row < res.rows; row++)
        {
            for (int col = 0; col < res.cols; col++)
            {
                int val = 0;

                for (int mid = 0; mid < left.cols; mid++)
                    val += left.data[row][mid] * right.data[mid][col];

                res.data[row][col] = val;
            }
        }
    }

    return res;
}

struct matrix mat_pow(struct matrix mat, int q)
{
    struct matrix res, temp;

    if (q == 0)
    {
        res = mat_create(mat.rows, mat.cols);
        for (int row = 0; row < res.rows; row++)
            for (int col = 0; col < res.cols; col++)
                res.data[row][col] = row == col;
    }
    else if (q == 1)
        return mat;
    else
    {
        res = mat_mult(mat, mat);
        q--;

        temp = res;
        while (--q > 0)
        {
            res = mat_mult(res, mat);
            mat_destroy(temp);
            temp = res;
        }
    }

    mat_destroy(mat);
    return res;
}
