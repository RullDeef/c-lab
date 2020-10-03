#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "status_codes.h"
#include "item.h"
#include "itemio.h"

#define TEMP_STR_SIZE 256

status_code_t imp__read_next_str_file(FILE *file, char *str, int size);
status_code_t imp__read_next_float_file(FILE *file, float *number);
status_code_t imp__read_next_item_file(FILE *file, item_t *item);
status_code_t imp__read_items_file(FILE *file, item_t *items, short int *items_count);

/**
 *  @return success - была успешно считана структура целиком,
 *          failure - сбой при считывании имени,
 *          end_of_file_reached - файл уже прочитан полностью.
 */
status_code_t imp__read_next_str_file(FILE *file, char *str, int size)
{
    if (fgets(str, size, file) != str)
        return feof(file) ? failure : end_of_file_reached;

    // truncate '\n' & '\r' symbols
    while (strlen(str) != 0 && (str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == '\r'))
        str[strlen(str) - 1] = '\0';

    return strlen(str) == 0 ? failure : success;
}

status_code_t imp__read_next_float_file(FILE *file, float *number)
{
    char temp[TEMP_STR_SIZE];
    char *ptr = NULL;
    status_code_t result;

    if (fgets(temp, TEMP_STR_SIZE, file) != temp)
        result = feof(file) ? end_of_file_reached : failure;
    else
    {
        *number = strtof(temp, &ptr);
        result = ptr == NULL ? failure : success;
    }

    return result;
}

status_code_t imp__read_next_item_file(FILE *file, item_t *item)
{
    status_code_t result = imp__read_next_str_file(file, item->name, MAX_ITEM_NAME_SIZE);

    if (result == success)
    {
        if (imp__read_next_float_file(file, &item->mass) || imp__read_next_float_file(file, &item->volume))
            result = failure;
    }
    else if (result == failure)
    {
        // try to detect end of file
        char temp[TEMP_STR_SIZE];
        fgets(temp, TEMP_STR_SIZE, file);
        if (feof(file))
            result = end_of_file_reached;
    }

    return result;
}

status_code_t imp__read_items_file(FILE *file, item_t *items, short int *items_count)
{
    item_t temp_item;
    status_code_t result = success;

    *items_count = 0;

    while (*items_count != MAX_ITEMS_AMOUNT && result != end_of_file_reached)
    {
        // try to read next item
        result = imp__read_next_item_file(file, &temp_item);

        if (result != end_of_file_reached)
        {
            // assign readen item to items array
            items[*items_count] = temp_item;
            (*items_count)++;
        }
    }

    // if max items count is reached and there is no eof - return error code
    if (*items_count == MAX_ITEMS_AMOUNT || result == end_of_file_reached)
        result = result == end_of_file_reached ? success : max_items_amount_reached;

    return result;
}

status_code_t read_items(const char *filename, item_t *items, short int *items_count)
{
    FILE *file = fopen(filename, "rt");
    status_code_t result = success;

    if (!file)
        result = cant_open_file;
    else
    {
        result = imp__read_items_file(file, items, items_count);
        fclose(file);
    }

    return result;
}

void print_array(item_t *items, short int items_count)
{
    for (short int i = 0; i < items_count; i++)
        printf("%s\n%.6f\n%.6f\n", items[i].name, items[i].mass, items[i].volume);
}
