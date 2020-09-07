#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ITEM_NAME_SIZE 26
#define MAX_ITEMS_AMOUNT 15
#define ALL_PREFIX "ALL"

typedef enum
{
    SUCCESS,
    FAILURE,
    TOO_FEW_ARGS,
    INVALID_ARGS,
    CANT_OPEN_FILE,
    CANT_READ_ITEMS,
    CANT_PROCESS_ITEMS,
    MAX_ITEMS_AMOUT_REACHED,
    MAX_ITEM_NAME_SIZE_REACHED,
    INVALID_ITEM_NAME,
    INVALID_ITEM_VOLUME,
    INVALID_ITEM,
    END_OF_FILE_REACHED
} status_code_t;

typedef struct
{
    char name[MAX_ITEM_NAME_SIZE];
    float mass;
    float volume;
    float density;
} item_t;

status_code_t read_cmdline_args(int argc, const char **argv, const char **filename, const char **prefix)
{
    if (argc == 1)
        return TOO_FEW_ARGS;

    *filename = argv[1];
    *prefix = argc >= 3 ? argv[2] : NULL;

    // if (*prefix != NULL && (strchr(*prefix, '>') != NULL || strchr(*prefix, '<') != NULL))
    //    *prefix = NULL;

    return SUCCESS;
}

/**
 *  @return SUCCESS - была успешно считана структура целиком,
 *          FAILURE - сбой при считывании имени или чисел,
 *          END_OF_FILE_REACHED - файл уже прочитан полностью.
 */
status_code_t read_next_str(FILE *file, char *str, int size)
{
    if (fgets(str, size, file) != str)
        return feof(file) ? FAILURE : END_OF_FILE_REACHED;

    // truncate '\n' & '\r' symbols
    while (strlen(str) != 0 && (str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == '\r'))
        str[strlen(str) - 1] = '\0';
    
    return strlen(str) == 0 ? FAILURE : SUCCESS;
}

status_code_t read_next_float(FILE *file, float *number)
{
    char str[256];
    if (fgets(str, 256, file) != str)
        return feof(file) ? END_OF_FILE_REACHED : FAILURE;
    
    char *ptr = NULL;
    *number = strtof(str, &ptr);
    return ptr == NULL ? FAILURE : SUCCESS;
}

status_code_t read_next_item_file(FILE *file, item_t *item)
{
    status_code_t result;
    if ((result = read_next_str(file, item->name, MAX_ITEM_NAME_SIZE)) != SUCCESS)
    {
        if (result == FAILURE)
        {
            // try to detect end of file
            char temp[256];
            fgets(temp, 256, file);
            if (feof(file))
                result = END_OF_FILE_REACHED;
        }
        
        return result;
    }

    if (read_next_float(file, &item->mass) || read_next_float(file, &item->volume))
        return FAILURE;

    return SUCCESS;
}

status_code_t read_items_file(FILE *file, item_t *items, size_t *items_count)
{
    item_t temp_item;
    *items_count = 0;
    status_code_t result = SUCCESS;

    while (true)
    {
        // try to read next item
        result = read_next_item_file(file, &temp_item);

        // if max items count is reached and there is no eof - return error code
        if (*items_count == MAX_ITEMS_AMOUNT && result != END_OF_FILE_REACHED)
            return MAX_ITEMS_AMOUT_REACHED;
        else if (result == END_OF_FILE_REACHED)
            return SUCCESS;

        // assign readen item to items array
        items[*items_count] = temp_item;
        (*items_count)++;
    }

    /*
    while (*items_count < MAX_ITEMS_AMOUNT && (result = read_next_item_file(file, &items[*items_count])) == 0)
        (*items_count)++;

    if (*items_count == MAX_ITEMS_AMOUNT)
    {
        // if file has more data
        item_t temp_item;
        if (read_next_item_file(file, &temp_item) != END_OF_FILE_REACHED)
            return MAX_ITEMS_AMOUT_REACHED;
    }

    if (result != END_OF_FILE_REACHED)
        return FAILURE;
    
    return SUCCESS;
    */
}

status_code_t read_items(const char *filename, item_t *items, size_t *items_count)
{
    FILE *file = fopen(filename, "rt");
    status_code_t result = SUCCESS;

    if (!file)
        return CANT_OPEN_FILE;

    if (read_items_file(file, items, items_count))
        result = CANT_READ_ITEMS;

    fclose(file);
    return result;
}

void swap(item_t *item_1, item_t *item_2)
{
    item_t temp = *item_1;
    *item_1 = *item_2;
    *item_2 = temp;
}

status_code_t sort_array(item_t *items, size_t items_count)
{
    for (size_t i = 1; i < items_count; i++)
        for (size_t j = i - 1; j >= 0 && (items[j].density > items[j + 1].density); j--)
            swap(&items[j], &items[j + 1]);
    
    return SUCCESS;
}

void pop_item(item_t *items, size_t *items_count, size_t index)
{
    for (size_t i = index; i + 1 < *items_count; i++)
        items[i] = items[i + 1];
    (*items_count)--;
}

void filter_array(item_t *items, size_t *items_count, const char *prefix)
{
    for (size_t i = 0; i < *items_count; i++)
        if (strstr(items[i].name, prefix) != items[i].name)
            pop_item(items, items_count, i--);
}

bool has_invalid_items(item_t *items, size_t items_count)
{
    for (int i = 0; i < items_count; i++)
        if (items[i].mass <= 0.0f || items[i].volume <= 0.0f)
            return true;
    return false;
}

void calculate_density(item_t *items, size_t items_count)
{
    for (size_t i = 0; i < items_count; i++)
        items[i].density = items[i].mass / items[i].volume;
}

status_code_t process_items(item_t *items, size_t *items_count, const char *prefix)
{
    if (has_invalid_items(items, *items_count))
        return INVALID_ITEM;
    
    calculate_density(items, *items_count);

    if (prefix == NULL)
        return sort_array(items, *items_count);
    
    else if (strcmp(prefix, ALL_PREFIX) != 0)
        filter_array(items, items_count, prefix);
    
    return SUCCESS;
}

void print_array(item_t *items, size_t items_count)
{
    for (size_t i = 0; i < items_count; i++)
        printf("%s\n%.6f\n%.6f\n", items[i].name, items[i].mass, items[i].volume);
}

int main(int argc, const char **argv)
{
    const char *filename = NULL;
    const char *prefix = NULL;
    item_t items[MAX_ITEMS_AMOUNT];
    size_t items_count;

    if (read_cmdline_args(argc, argv, &filename, &prefix))
        return INVALID_ARGS;

    if (read_items(filename, items, &items_count))
        return CANT_READ_ITEMS;
    
    if (process_items(items, &items_count, prefix))
        return CANT_PROCESS_ITEMS;
    
    print_array(items, items_count);
    return SUCCESS;
}