#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "item.h"
#include "item_io.h"

#define MAX_PRICE_STR_LEN 128
#define MAX_ITEMS_COUNT_STR_LEN 128

static void imp__mark_file_eof(FILE *file)
{
    int c;
    if ((c = fgetc(file)) != EOF)
        ungetc(c, file);
}

static void imp__skip_items_count(FILE *file)
{
    char tmp[MAX_ITEMS_COUNT_STR_LEN];
    fgets(tmp, MAX_ITEMS_COUNT_STR_LEN, file);
}

static int imp__read_price(FILE *file, unsigned int *price)
{
    char tmp[MAX_PRICE_STR_LEN];
    int status = EXIT_FAILURE;
    int count;

    if (fgets(tmp, MAX_PRICE_STR_LEN, file) != NULL &&
            sscanf(tmp, "%u %n", price, &count) == 1 &&
            (size_t)count == strlen(tmp))
    {
        imp__mark_file_eof(file);
        status = EXIT_SUCCESS;
    }

    return status;
}

struct item it_read_file(FILE *file)
{
    char *name = NULL;
    unsigned int price = 0U;
    size_t n = 0U;
    ssize_t len;

    if ((len = getline(&name, &n, file)) != -1)
    {
        while (len > 0U && isspace((int)name[len - 1]))
            name[len - 1] = '\0';

        if (len == 0 || imp__read_price(file, &price) != EXIT_SUCCESS)
        {
            free(name);
            name = NULL;
        }
    }
    else
    {
        free(name);
        name = NULL;
    }

    return it_construct(name, price);
}

void it_print_file(FILE *file, const struct item *it)
{
    fprintf(file, "%s\n%u\n", it->name, it->price);
}

struct item_array it_read_array_file(FILE *file)
{
    struct item_array ita = ita_create();

    imp__skip_items_count(file);
    while (ita_is_valid(&ita) && !feof(file))
    {
        struct item it = it_read_file(file);
        if (it_is_valid(&it))
            ita_insert(&ita, &it);
        else
            ita_destroy(&ita);
    }

    if (ita_is_valid(&ita) && ita.size == 0U)
        ita_destroy(&ita);

    return ita;
}

void it_print_array_file_constraint(FILE *file, const struct item_array *ita, float price)
{
    for (unsigned int i = 0U; i < ita->size; i++)
    {
        struct item *it = ita->data + i;
        if (it->price < price)
            it_print_file(file, it);
    }
}

struct item_array it_read_array(const char *fname)
{
    struct item_array ita = ita_invalid();

    FILE *file = fopen(fname, "rt");
    if (file != NULL)
    {
        ita = it_read_array_file(file);
        fclose(file);
    }

    return ita;
}
