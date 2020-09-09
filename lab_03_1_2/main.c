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
    invalid_element_input,
    invalid_matrix_struct
} status_code_t;


bool is_valid_dims(size_t rows, size_t columns);
status_code_t input_data(matrix_t matrix, size_t *rows, size_t *columns);
void print_status_code(status_code_t status_code);
int digit_sum(int number);
bool is_correct_matrix(matrix_t matrix, size_t rows, size_t columns);
void find_min_digit_sum(matrix_t matrix, size_t rows, size_t columns, size_t *row, size_t *column);
void assign_array(array_t dest, array_t src, size_t size);
void delete_matrix_row(matrix_t matrix, size_t *rows, size_t columns, size_t row_to_delete);
void delete_array_element(array_t array, size_t size, size_t index);
void delete_matrix_column(matrix_t matrix, size_t rows, size_t *columns, size_t column_to_delete);
void print_matrix(matrix_t matrix, size_t rows, size_t columns);


int main(void)
{
    matrix_t matrix;
    size_t rows;
    size_t columns;

    size_t min_el_row;
    size_t min_el_column;

    status_code_t result = exit_success;

    if ((result = input_data(matrix, &rows, &columns)))
        print_status_code(result);
    else
    {
        find_min_digit_sum(matrix, rows, columns, &min_el_row, &min_el_column);
        delete_matrix_row(matrix, &rows, columns, min_el_row);
        delete_matrix_column(matrix, rows, &columns, min_el_column);

        print_matrix(matrix, rows, columns);
    }

    return result;
}


bool is_valid_dims(size_t rows, size_t columns)
{
    bool rows_valid = 1 < rows && rows < MAX_CAPACITY;
    bool columns_valid = 1 < columns && columns < MAX_CAPACITY;
    return rows_valid && columns_valid;
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

        if (result == exit_success)
            if (!is_correct_matrix(matrix, *rows, *columns))
                result = invalid_matrix_struct;
    }

    return result;
}


void print_status_code(status_code_t status_code)
{
    if (status_code == invalid_input)
        printf("incorrect input.\n");
    else if (status_code == invalid_matrix_dimentions)
        printf("incorrect matrix dimentions.\n");
    else if (status_code == invalid_element_input)
        printf("incorrect element input.\n");
    else if (status_code == invalid_matrix_struct)
        printf("incorrect matrix structure.\n");
}


int digit_sum(int number)
{
    int result = 0;

    number = number < 0 ? -number : number;
    while (number > 0)
    {
        result += number % 10;
        number /= 10;
    }

    return result;
}


bool is_correct_matrix(matrix_t matrix, size_t rows, size_t columns)
{
    int element;
    int curr_digit_sum;
    int min_digit_sum = -1;
    size_t min_amount = 1;

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t column = 0; column < columns; column++)
        {
            element = matrix[row][column];
            curr_digit_sum = digit_sum(element);

            if (min_digit_sum == -1)
                min_digit_sum = curr_digit_sum;
            else if (curr_digit_sum == min_digit_sum)
                min_amount++;
            else if (curr_digit_sum < min_digit_sum)
            {
                min_digit_sum = curr_digit_sum;
                min_amount = 1;
            }
        }
    }

    return min_amount == 1;
}


void find_min_digit_sum(matrix_t matrix, size_t rows, size_t columns, size_t *row, size_t *column)
{
    int element;
    int curr_digit_sum;
    int min_digit_sum = -1;

    for (size_t r = 0; r < rows; r++)
    {
        for (size_t c = 0; c < columns; c++)
        {
            element = matrix[r][c];
            curr_digit_sum = digit_sum(element);

            if (min_digit_sum == -1 || curr_digit_sum < min_digit_sum)
            {
                min_digit_sum = curr_digit_sum;
                *row = r;
                *column = c;
            }
        }
    }
}


void assign_array(array_t dest, array_t src, size_t size)
{
    for (size_t i = 0; i < size; i++)
        dest[i] = src[i];
}


void delete_matrix_row(matrix_t matrix, size_t *rows, size_t columns, size_t row_to_delete)
{
    *rows -= 1;

    for (size_t row = row_to_delete; row < *rows; row++)
        assign_array(matrix[row], matrix[row + 1], columns);
}


void delete_array_element(array_t array, size_t size, size_t index)
{
    for (size_t i = index; i + 1 < size; i++)
        array[i] = array[i + 1];
}


void delete_matrix_column(matrix_t matrix, size_t rows, size_t *columns, size_t column_to_delete)
{
    for (size_t row = 0; row < rows; row++)
        delete_array_element(matrix[row], *columns, column_to_delete);
    *columns -= 1;
}


void print_matrix(matrix_t matrix, size_t rows, size_t columns)
{
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            printf(column + 1 < columns ? "%d " : "%d\n", matrix[row][column]);
}