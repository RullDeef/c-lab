#include <stdio.h>
#include <string.h>

#define MAX_ITEM_NAME_SIZE 26
#define MAX_ITEMS_AMOUNT 128
#define ALL_PREFIX "ALL"

typedef struct
{
    char name[MAX_ITEM_NAME_SIZE];
    float mass;
    float volume;
    float density;
} item_t;

int read_cmdline_args(int argc, const char **argv, const char **filename, const char **prefix)
{
    if (argc == 1)
        return 1;

    *filename = argv[1];
    *prefix = argc >= 3 ? argv[2] : NULL;
    if (*prefix != NULL && (strchr(*prefix, '>') != NULL || strchr(*prefix, '<') != NULL))
        *prefix = NULL;

    return 0;
}

int read_next_item_file(FILE *file, item_t *item)
{
    if (fgets(item->name, MAX_ITEM_NAME_SIZE, file) != item->name)
        return feof(file) ? 1 : -1;
    
    // truncate '\n' & '\r' symbols
    while (item->name[strlen(item->name) - 1] == '\n' || item->name[strlen(item->name) - 1] == '\r')
        item->name[strlen(item->name) - 1] = '\0';

    int scanned = fscanf(file, "%f %f ", &item->mass, &item->volume);
    if (scanned == 2)
        return 0;
    else if (scanned == EOF)
        return 1;
    else
        return -1;
}

int read_items_file(FILE *file, item_t *items, size_t *items_count)
{
    *items_count = 0;
    int result = 0;

    while (*items_count < MAX_ITEMS_AMOUNT && (result = read_next_item_file(file, &items[*items_count])) == 0)
        (*items_count)++;

    if (*items_count == MAX_ITEMS_AMOUNT)
        return -1;

    if (result != 1)
        return -2;
    
    return 0;
}

int read_items(const char *filename, item_t *items, size_t *items_count)
{
    FILE *file = fopen(filename, "rt");
    int result = 0;

    if (!file)
        return -1;

    if (read_items_file(file, items, items_count))
        result = -2;

    fclose(file);
    return result;
}

void swap(item_t *item_1, item_t *item_2)
{
    item_t temp = *item_1;
    *item_1 = *item_2;
    *item_2 = temp;
}

int sort_array(item_t *items, size_t items_count)
{
    for (size_t i = 0; i < items_count; i++)
    {
        if (items[i].volume == 0.0f)
            return -1;
        
        items[i].density = items[i].mass / items[i].volume;
    }

    for (size_t i = 1; i < items_count; i++)
        for (size_t j = i - 1; j >= 0 && (items[j].density > items[j + 1].density); j--)
            swap(&items[j], &items[j + 1]);
    
    return 0;
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

int process_items(item_t *items, size_t* items_count, const char *prefix)
{
    if (prefix == NULL)
        return sort_array(items, *items_count);
    
    else if (strcmp(prefix, ALL_PREFIX) != 0)
        filter_array(items, items_count, prefix);
    
    return 0;
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
        return -1;

    if (read_items(filename, items, &items_count))
        return -2;
    
    if (process_items(items, &items_count, prefix))
        return -3;
    
    print_array(items, items_count);
    return 0;
}