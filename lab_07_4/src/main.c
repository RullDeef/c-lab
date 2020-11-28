#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "status_codes.h"
#include "filter_func.h"
#include "sort_func.h"
#include "utils.h"

/**
 * @brief Константа для опции фильтрации, передаваемой через командную строку
 */
#define FILTER_OPT_STR "f"

/**
 * @brief Обрабатывает аргументы командной строки
 * 
 * @param argc - кол-во аргументов командной строки
 * @param argv - массив из строк - аргументов командной строки
 * @param input_filename - имя входного файла
 * @param output_filename - имя выходного файла
 * @param need_filtration - флаг необходимости фильтрации
 * 
 * @return  invalid_args - неверное кол-во агрументов командной строки
 *          invalid_opt - невеопознанный аргумент командной строки
 *          success - успешное выполнение
 */
int parse_args(int argc, char **argv, char **input_filename, char **output_filename, bool *need_filtration);

/**
 * @brief Фильтрует и сортирует массив чисел
 * 
 * После фильтрации удаляет старый массив (освобождает память)
 * и замещает его новым массивом, созданным после фильтрации
 * 
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (на элемент за последним)
 * @param need_filtration - флаг необходимости фильтрации
 * 
 * @return  invalid_args - некорректные аргументы функции
 *          invalid_ptrs - некорректные указатели на начало и конец массива
 *          invalid_elements_amount - нет оставшихся после фильтрации элементов
 *          bad_alloc - неудача при попытке динамического выделения памяти
 *          success - успешное выполнение
 */
int process(int **begin, int **end, bool need_filtration);

int main(int argc, char **argv)
{
    char *input_filename = NULL;
    char *output_filename = NULL;

    bool need_filtration = false;

    int status_code = parse_args(argc, argv, &input_filename, &output_filename, &need_filtration);

    if (status_code == success)
    {
        int *data_array_begin = NULL;
        int *data_array_end = NULL;
        
        if (read_data_file(input_filename, &data_array_begin, &data_array_end))
            status_code = failure;
        else
        {
            status_code = process(&data_array_begin, &data_array_end, need_filtration);

            if (status_code == success)
                status_code = write_file(output_filename, data_array_begin, data_array_end);

            free(data_array_begin);
        }
    }

    return status_code;
}

int parse_args(int argc, char **argv, char **input_filename, char **output_filename, bool *need_filtration)
{
    int status_code = success;

    if (argc != 3 && argc != 4)
        status_code = invalid_args;
    else
    {
        *input_filename = argv[1];
        *output_filename = argv[2];

        *need_filtration = false;
        if (argc == 4)
        {
            if (strcmp(argv[3], FILTER_OPT_STR) == 0)
                *need_filtration = true;
            else
                status_code = invalid_opt; // unrecognized option
        }
    }

    return status_code;
}

int process(int **begin, int **end, bool need_filtration)
{
    int status_code = success;

    if (need_filtration)
    {
        int *filtered_begin;
        int *filtered_end;

        status_code = key(*begin, *end, &filtered_begin, &filtered_end);
        if (status_code == success)
        {
            free(*begin);
            *begin = filtered_begin;
            *end = filtered_end;
        }
    }

    if (status_code == success)
        status_code = mysort(*begin, *end - *begin, sizeof(int), int_comparator);

    return status_code;
}
