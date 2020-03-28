#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdbool.h>


#define MAX_CAPACITY 10
typedef int array_t[MAX_CAPACITY];
typedef array_t matrix_t[MAX_CAPACITY];


typedef enum
{
    exit_success,
    exit_failure,
    invalid_input,
    invalid_matrix_dimentions,
    invalid_element_input
} status_code_t;


status_code_t input_data(matrix_t matrix, size_t *rows, size_t *columns);
bool is_valid_dims(size_t rows, size_t columns);
void print_error_msg(status_code_t status_code);
bool is_mono_seq(array_t array, size_t size);
void print_mono_seqs(matrix_t matrix, size_t rows, size_t columns);


int main(void)
{
    matrix_t matrix;
    size_t rows;
    size_t columns;

    status_code_t result = exit_success;

    if ((result = input_data(matrix, &rows, &columns)))
        print_error_msg(result);
    else
        print_mono_seqs(matrix, rows, columns);

    return result;
}


status_code_t input_data(matrix_t matrix, size_t *rows, size_t *columns)
{
    status_code_t result = exit_success;

    if (scanf("%zu%zu", rows, columns) != 2)
        result = invalid_input;
    else if (!is_valid_dims(*rows, *columns))
        result = invalid_matrix_dimentions;
    else
    {
        for (size_t row = 0; row < *rows; row++)
        {
            for (size_t column = 0; column < *columns; column++)
            {
                if (scanf("%d", &matrix[row][column]) != 1)
                {
                    result = invalid_element_input;
                    break;
                }
            }

            if (result)
                break;
        }
    }

    return result;
}


bool is_valid_dims(size_t rows, size_t columns)
{
    bool rows_valid = 0 < rows && rows < MAX_CAPACITY;
    bool columns_valid = 1 < columns && columns < MAX_CAPACITY;
    return rows_valid && columns_valid;
}


void print_error_msg(status_code_t status_code)
{
    if (status_code == invalid_input)
        printf("incorrect input.\n");
    else if (status_code == invalid_matrix_dimentions)
        printf("incorrect matrix dimentions.\n");
    else if (status_code == invalid_element_input)
        printf("incorrect element input.\n");
}


bool is_mono_seq(array_t array, size_t size)
{
    int delta_1;
    int delta_2;
    bool result = array[0] != array[1];

    if (size > 2)
    {
        for (size_t i = 2; i < size; i++)
        {
            delta_1 = array[i - 1] - array[i - 2];
            delta_2 = array[i] - array[i - 1];
            if (delta_1 * delta_2 < 0)
            {
                result = false;
                break;
            }
        }
    }

    return result;
}


void print_mono_seqs(matrix_t matrix, size_t rows, size_t columns)
{
    for (size_t row = 0; row < rows; row++)
    {
        bool mono = is_mono_seq(matrix[row], columns);
        printf(mono ? "1" : "0");
        printf(row == rows - 1 ? "\n" : " ");
    }
}