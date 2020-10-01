#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include <sys/time.h>
#include "status_codes.h"
#include "filter_func.h"
#include "sort_func.h"

/**
 * @brief Константа для опции фильтрации, передаваемой через командную строку
 */
#define FILTER_OPT_STR "f"

/**
 * @brief Выводит массив чисел в файл
 * 
 * @param file - файл для вывода
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (элемент за последним)
 */
void print_array_to_file(FILE *file, int *begin, int *end);

/**
 * @brief Выводит массив чисел в файл
 * 
 * @param filename - имя файла для записи
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (элемент за последним)
 * 
 * @return  cant_open_file - невозможно открыть файл для записи
 *          success - успешное выполнение
 */
int write_file(char *filename, int *begin, int *end);

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
int count_elements_in_file(FILE *file);

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
void read_elements_from_file(FILE *file, int **begin, int **end, int elements_count);

/**
 * @brief Считывает числа из файла в массив
 * 
 * Создает и возвращает динамический массив чисел
 * 
 * @param filename - название файла для чтения
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (на элемент за последним)
 * 
 * @return  cant_open_file - невозможно открыть файл
 *          invalid_elements_amount - неверная структура файла
 *          bad_alloc - неудачная попытка динамически выделить память
 *          success - успешное выполнение
 */
int read_data_file(char *filename, int **begin, int **end);

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
int do_tasks(int **begin, int **end, bool need_filtration);

/**
 * @brief Считывает массив чисел из файла, фильтрует и
 * сортирует его, после чего записывает в выходной файл
 * 
 * @param input_filename - имя входного файла
 * @param output_filename - имя выходного файла
 * @param need_filtration - флаг необходимости фильтрации
 * 
 * @return  failure - неудача при считывании массива чисел из файла
 *          invalid_args - некорректные аргументы функции
 *          invalid_ptrs - некорректные указатели на начало и конец массива
 *          invalid_elements_amount - нет оставшихся после фильтрации элементов
 *          bad_alloc - неудача при попытке динамического выделения памяти
 *          success - успешное выполнение
 */
int proccess(char *input_filename, char *output_filename, bool need_filtration);

int main(int argc, char **argv)
{
    char *input_filename = NULL;
    char *output_filename = NULL;

    bool need_filtration = false;

    int status_code = parse_args(argc, argv, &input_filename, &output_filename, &need_filtration);

    if (status_code == success)
        status_code = proccess(input_filename, output_filename, need_filtration);

    return status_code;
}

void print_array_to_file(FILE *file, int *begin, int *end)
{
    fprintf(file, "%d", *begin);
    for (begin++; begin != end; begin++)
        fprintf(file, " %d", *begin);
}

int write_file(char *filename, int *begin, int *end)
{
    int status_code = success;

    FILE *file = fopen(filename, "wt");
    if (file != NULL)
    {
        print_array_to_file(file, begin, end);
        fclose(file);
    }
    else
        status_code = cant_open_file;

    return status_code;
}

int count_elements_in_file(FILE *file)
{
    int elements_count = 0;

    int temp, rc;
    while ((rc = fscanf(file, "%i ", &temp)) == 1)
        elements_count++;

    return rc == EOF ? elements_count : failure;
}

void read_elements_from_file(FILE *file, int **begin, int **end, int elements_count)
{
    rewind(file);
    *end = *begin;
    while (*end != *begin + elements_count)
        fscanf(file, "%i", (*end)++);
}

int read_data_file(char *filename, int **begin, int **end)
{
    int status_code = success;

    FILE *file = fopen(filename, "rt");
    if (file == NULL)
        status_code = cant_open_file;
    else
    {
        int elements_count = count_elements_in_file(file);
        if (elements_count <= 0)
            status_code = invalid_elements_amount;
        else
        {
            *begin = (int *)malloc(elements_count * sizeof(int));
            if (*begin == NULL)
                status_code = bad_alloc;
            else
                read_elements_from_file(file, begin, end, elements_count);
        }

        fclose(file);
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

int do_tasks(int **begin, int **end, bool need_filtration)
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
    {
        // struct timeval start_tv, end_tv;
        // struct timezone tz;
        // gettimeofday(&start_tv, &tz);
        status_code = mysort(*begin, *end - *begin, sizeof(int), int_comparator);
        // qsort(*begin, *end - *begin, sizeof(int), int_comparator);
        // gettimeofday(&end_tv, &tz);
        // unsigned long start_time = 1000000 * start_tv.tv_sec + start_tv.tv_usec;
        // unsigned long end_time = 1000000 * end_tv.tv_sec + end_tv.tv_usec;
        // fprintf(stderr, "%lu\n", end_time - start_time);
    }

    return status_code;
}

int proccess(char *input_filename, char *output_filename, bool need_filtration)
{
    int *data_array_begin = NULL;
    int *data_array_end = NULL;
    int status_code = success;

    if (read_data_file(input_filename, &data_array_begin, &data_array_end))
        status_code = failure;
    else
    {
        status_code = do_tasks(&data_array_begin, &data_array_end, need_filtration);

        if (status_code == success)
            status_code = write_file(output_filename, data_array_begin, data_array_end);

        free(data_array_begin);
    }

    return status_code;
}
