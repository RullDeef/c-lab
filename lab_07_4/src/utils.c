#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "status_codes.h"

/**
 * @brief Подсчитывает кол-во чисел в файле
 * 
 * После работы с файлом, не переводит файловый указатель в начало
 * 
 * @param file - файл для чтения
 * 
 * @return  -1 - файл имеет некорректную структуру
 *          elements_count - кол-во чисел в файле
 */
int imp__count_elements_in_file(FILE *file);

/**
 * @brief Выводит массив чисел в файл
 * 
 * @param file - файл для вывода
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (элемент за последним)
 */
void imp__print_array_to_file(FILE *file, int *begin, int *end);

/**
 * @brief Считывает elements_count чисел из файла в массив
 * 
 * Массив должен быть доступен для использования до вызова функции
 * 
 * После работы устанавливает указатель end на конец массива
 * 
 * @param file - файл для чтения
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (элемент за последним)
 * @param elements_count - кол-во элементов для считывания
 */
void imp__read_elements_from_file(FILE *file, int **begin, int **end, int elements_count);

int write_file(char *filename, int *begin, int *end)
{
    int status_code = success;

    FILE *file = fopen(filename, "wt");
    if (file != NULL)
    {
        imp__print_array_to_file(file, begin, end);
        fclose(file);
    }
    else
        status_code = cant_open_file;

    return status_code;
}

int read_data_file(char *filename, int **begin, int **end)
{
    int status_code = success;

    FILE *file = fopen(filename, "rt");
    if (file == NULL)
        status_code = cant_open_file;
    else
    {
        int elements_count = imp__count_elements_in_file(file);
        if (elements_count <= 0)
            status_code = invalid_elements_amount;
        else
        {
            *begin = (int *)malloc(elements_count * sizeof(int));
            if (*begin == NULL)
                status_code = bad_alloc;
            else
                imp__read_elements_from_file(file, begin, end, elements_count);
        }

        fclose(file);
    }

    return status_code;
}

void imp__print_array_to_file(FILE *file, int *begin, int *end)
{
    fprintf(file, "%d", *begin);
    for (begin++; begin != end; begin++)
        fprintf(file, " %d", *begin);
}

int imp__count_elements_in_file(FILE *file)
{
    int elements_count = 0;

    int temp, rc;
    while ((rc = fscanf(file, "%i ", &temp)) == 1)
        elements_count++;

    return rc == EOF ? elements_count : failure;
}

void imp__read_elements_from_file(FILE *file, int **begin, int **end, int elements_count)
{
    rewind(file);
    *end = *begin;
    while (*end != *begin + elements_count)
        fscanf(file, "%i", (*end)++);
}
