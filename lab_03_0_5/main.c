#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdbool.h>


#define MAX_CAPACITY 10
#define ULTIMATE_CAPACITY (MAX_CAPACITY * MAX_CAPACITY)
typedef int array_t[ULTIMATE_CAPACITY];
typedef int matrix_t[MAX_CAPACITY][MAX_CAPACITY];


typedef enum
{
    exit_success,
    exit_failure,
    invalid_input,
    invalid_matrix_dimentions,
    invalid_element_input,
    invalid_matrix_struct
} status_code_t;


bool is_prime(int number);
bool is_valid_dims(size_t rows, size_t columns);
status_code_t input_data(matrix_t matrix, size_t *rows, size_t *columns);
void print_error_msg(status_code_t status_code);
void preserve_zeros(matrix_t matrix, size_t rows, size_t columns);
void restore_zeros(matrix_t matrix, size_t rows, size_t columns);
void extract_primes(matrix_t matrix, size_t rows, size_t columns, array_t array, size_t *size);
void reverse_array(array_t array, size_t size);
void insert_primes(matrix_t matrix, size_t rows, size_t columns, array_t array, size_t size);
void print_matrix(matrix_t matrix, size_t rows, size_t columns);


int main(void)
{
    matrix_t matrix;
    size_t rows;
    size_t columns;

    array_t primes_array;
    size_t size;

    status_code_t result = exit_success;

    if ((result = input_data(matrix, &rows, &columns)))
        print_error_msg(result);
    else
    {
        preserve_zeros(matrix, rows, columns);

        extract_primes(matrix, rows, columns, primes_array, &size);
        reverse_array(primes_array, size);
        insert_primes(matrix, rows, columns, primes_array, size);

        restore_zeros(matrix, rows, columns);
        
        print_matrix(matrix, rows, columns);
    }

    return result;
}


bool is_prime(int number)
{
    bool result = true;

    if (number == 1)
        result = false;
    else
    {
        for (int i = 2; i * i <= number; i++)
        {
            if (number % i == 0)
            {
                result = false;
                break;
            }
        }
    }

    return result;
}

status_code_t input_data(matrix_t matrix, size_t *rows, size_t *columns)
{
    bool has_prime = false;

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

                has_prime = has_prime || is_prime(matrix[row][column]);
            }

            if (result)
                break;
        }
    }

    if (result == exit_success && !has_prime)
        result = invalid_matrix_struct;

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
    else if (status_code == invalid_matrix_struct)
        printf("incorrect matrix structure.\n");
}


void preserve_zeros(matrix_t matrix, size_t rows, size_t columns)
{
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            if (matrix[row][column] <= 0)
                matrix[row][column]--;
}


void restore_zeros(matrix_t matrix, size_t rows, size_t columns)
{
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            if (matrix[row][column] < 0)
                matrix[row][column]++;
}


void extract_primes(matrix_t matrix, size_t rows, size_t columns, array_t array, size_t *size)
{
    *size = 0;

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t column = 0; column < columns; column++)
        {
            if (is_prime(matrix[row][column]))
            {
                array[*size] = matrix[row][column];
                matrix[row][column] = 0;
                *size += 1;
            }
        }
    }
}

void reverse_array(array_t array, size_t size)
{
    for (size_t i = 0; 2 * i < size; i++)
    {
        int temp = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = temp;
    }
}

void insert_primes(matrix_t matrix, size_t rows, size_t columns, array_t array, size_t size)
{
    size_t i = 0;

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t column = 0; column < columns; column++)
        {
            if (matrix[row][column] == 0)
            {
                matrix[row][column] = array[i];
                i++;
            }
        }
    }
}


void print_matrix(matrix_t matrix, size_t rows, size_t columns)
{
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            printf(column + 1 < columns ? "%d " : "%d\n", matrix[row][column]);
}