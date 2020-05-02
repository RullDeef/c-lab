#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdbool.h>

#define MAX_MATRIX_SIZE 20

typedef int matrix_t[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

typedef enum
{
    EXIT_SUCCESS = 0,
    INVALID_INPUT,
    INVALID_MATRIX_SIZE,
    INVALID_MATRIX_ELEMENT
} status_code_t;

typedef enum
{
    LOWER = -1,
    DIAGONAL,
    UPPER
} location_t;

status_code_t input_matrix_size(size_t *size);
status_code_t input_matrix(matrix_t matrix, const size_t size);
location_t get_location(const size_t size, const size_t diag_index, const size_t row, const size_t col);
int get_sum_in_location(matrix_t matrix, const size_t size, const size_t diag_index, location_t loc);
int upper_sum(matrix_t matrix, const size_t size, const size_t diag_index);
int lower_sum(matrix_t matrix, const size_t size, const size_t diag_index);
int find_best_diag_index(matrix_t matrix, const size_t size);
void print_diag_index(size_t diag_index);

int main(void)
{
    matrix_t matrix;
    size_t size;
    size_t diag_index;

    status_code_t result = EXIT_SUCCESS;

    if ((result = input_matrix_size(&size)) != EXIT_SUCCESS)
        ; // skip
    
    else if ((result = input_matrix(matrix, size)) != EXIT_SUCCESS)
        ; // skip
    
    else
    {
        diag_index = find_best_diag_index(matrix, size);
        print_diag_index(diag_index);
    }

    return result;
}

status_code_t input_matrix_size(size_t *size)
{
    status_code_t result = EXIT_SUCCESS;

    if (scanf("%zu", size) != 1)
        result = INVALID_INPUT;
    
    else if (*size <= 0 || *size > MAX_MATRIX_SIZE)
        result = INVALID_MATRIX_SIZE;
    
    return result;
}

status_code_t input_matrix(matrix_t matrix, const size_t size)
{
    status_code_t result = EXIT_SUCCESS;

    for (size_t row = 0; row < size; row++)
    {
        for (size_t col = 0; col < size; col++)
        {
            if (scanf("%d", &matrix[row][col]) != 1)
            {
                result = INVALID_MATRIX_ELEMENT;
                break;
            }
        }
    }

    return result;
}

location_t get_location(const size_t size, const size_t diag_index, const size_t row, const size_t col)
{
    size_t diag_number = size - 1 - col + row;
    location_t location;

    if (diag_number < diag_index)
        location = UPPER;
    
    else if (diag_number == diag_index)
        location = DIAGONAL;
    
    else
        location = LOWER;

    return location;
}

int get_sum_in_location(matrix_t matrix, const size_t size, const size_t diag_index, location_t loc)
{
    int sum = 0;

    for (size_t row = 0; row < size; row++)
    {
        for (size_t col = 0; col < size; col++)
        {
            loc = get_location(size, diag_index, row, col);

            if (col == LOWER)
                sum += matrix[row][col];
        }
    }

    return sum;
}

int get_upper_sum(matrix_t matrix, const size_t size, const size_t diag_index)
{
    int sum = get_sum_in_location(matrix, size, diag_index, UPPER);
    return sum;
}

int get_lower_sum(matrix_t matrix, const size_t size, const size_t diag_index)
{
    int sum = get_sum_in_location(matrix, size, diag_index, LOWER);
    return sum;
}

int find_best_diag_index(matrix_t matrix, const size_t size)
{
    size_t diag_amount = 2 * size - 1;
    size_t diag_index;
    bool founded = false;

    for (diag_index = 0; diag_index < diag_amount; diag_index++)
    {
        int upper_sum = get_upper_sum(matrix, size, diag_index);
        int lower_sum = get_lower_sum(matrix, size, diag_index);

        if (upper_sum == lower_sum)
        {
            founded = true;
            break;
        }
    }

    return founded ? diag_index : -1;
}

void print_diag_index(size_t diag_index)
{
    printf("%d", diag_index);
}
