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
void sort_matrix_rows(matrix_t matrix, size_t rows, size_t columns);
int get_min_element(array_t array, size_t size);
void swap_arrays(array_t arr_1, array_t array_2, size_t size);
void print_matrix(matrix_t matrix, size_t rows, size_t columns);


int main(void)
{
    matrix_t matrix;
    size_t rows;
    size_t columns;

    status_code_t result = exit_success;

    if ((result = input_data(matrix, &rows, &columns)))
        print_error_msg(result);
    else
    {
        sort_matrix_rows(matrix, rows, columns);
        print_matrix(matrix, rows, columns);
    }

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
    bool columns_valid = 0 < columns && columns < MAX_CAPACITY;
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


void sort_matrix_rows(matrix_t matrix, size_t rows, size_t columns)
{
    int min_el_1;
    int min_el_2;

    for (size_t row_1 = 0; row_1 + 1 < rows; row_1++)
    {
        min_el_1 = get_min_element(matrix[row_1], columns);
        for (size_t row_2 = row_1 + 1; row_2 < rows; row_2++)
        {
            min_el_2 = get_min_element(matrix[row_2], columns);
            if (min_el_1 < min_el_2) // do stable sort here
                for (size_t row_3 = row_1; row_3 < row_2; row_3++)
                    swap_arrays(matrix[row_3], matrix[row_3 + 1], columns);
        }
    }
}


int get_min_element(array_t array, size_t size)
{
    int min_element = array[0];

    for (size_t i = 0; i < size; i++)
        if (array[i] < min_element)
            min_element = array[i];
    
    return min_element;
}


void swap_arrays(array_t arr_1, array_t arr_2, size_t size)
{
    int temp;

    for (size_t i = 0; i < size; i++)
    {
        temp = arr_1[i];
        arr_1[i] = arr_2[i];
        arr_2[i] = temp;
    }
}


void print_matrix(matrix_t matrix, size_t rows, size_t columns)
{
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            printf(column + 1 < columns ? "%d " : "%d\n", matrix[row][column]);
}