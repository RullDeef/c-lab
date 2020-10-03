#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "status_codes.h"
#include "item.h"
#include "itemio.h"

#define ALL_PREFIX "ALL"

/**
 *  @brief Парсит аргументы командной строки.
 *  
 *  Выделяет имя входного файла и подстроку для фильтрации.
 *  Если подстроки нет, то она устанавливается в NULL.
 *  
 *  @param argc - кол-во аргуметов командной строки.
 *  @param argv - массив аргументов командной строки.
 *  @param filename - имя файла для чтения.
 *  @param prefix - подстрока для фильтрации.
 *  
 *  @return success - парсинг прошёл успешно,
 *          too_few_args - недостаточно аргументов командной строки,
 *          too_many_args - слишком много аргументов командной строки.
 */
status_code_t read_cmdline_args(int argc, const char **argv, const char **filename, const char **prefix);

/**
 *  @brief Обрабатывает массив предметов по условию задачи.
 *  
 *  @param items - массив предметов для обработки.
 *  @param items_count - кол-во элементов массива.
 *  @param prefix - подстрока для фильтрации.
 *  
 *  @return success - обработка завершена успешно,
 *          failure - после обработки не осталось данных,
 *          invalid_item - массив содержит некорректные структуры.
 */
status_code_t process_items(item_t *items, short int *items_count, const char *prefix);

int main(int argc, const char **argv)
{
    const char *filename = NULL;
    const char *prefix = NULL;
    item_t items[MAX_ITEMS_AMOUNT];
    short int items_count;

    if (read_cmdline_args(argc, argv, &filename, &prefix))
        return invalid_args;

    if (read_items(filename, items, &items_count))
        return cant_read_items;

    if (process_items(items, &items_count, prefix))
        return cant_process_items;

    print_array(items, items_count);
    return success;
}

status_code_t read_cmdline_args(int argc, const char **argv, const char **filename, const char **prefix)
{
    status_code_t result = success;

    if (argc == 1 || argc > 3)
        result = argc == 1 ? too_few_args : too_many_args;
    else
    {
        *filename = argv[1];
        *prefix = argc == 3 ? argv[2] : NULL;
    }

    return result;
}

status_code_t process_items(item_t *items, short int *items_count, const char *prefix)
{
    if (has_invalid_items(items, *items_count))
        return invalid_item;

    calculate_density(items, *items_count);

    if (prefix == NULL)
        sort_items_array(items, *items_count);
    else if (strcmp(prefix, ALL_PREFIX) != 0)
        filter_array(items, items_count, prefix);

    return *items_count > 0 ? success : failure;
}
