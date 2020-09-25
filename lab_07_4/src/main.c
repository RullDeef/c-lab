#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "status_codes.h"
#include "filter_func.h"
#include "sort_func.h"

#define FILTER_OPT_STR "f"


int write_file(char *filename, int *data_array_begin, int *data_array_end)
{
    assert(filename != NULL);
    assert(data_array_begin != NULL);
    assert(data_array_end != NULL);
    assert(data_array_begin < data_array_end);

    FILE *file = fopen(filename, "wt");
    if (file == NULL)
        return CANT_OPEN_FILE;

    fprintf(file, "%d", *data_array_begin);
    for (int *iter = data_array_begin + 1; iter != data_array_end; iter++)
        fprintf(file, " %d", *iter);

    fclose(file);
    return SUCCESS;
}

int count_elements_in_file(FILE *file)
{
    assert(file != NULL);

    int elements_count = 0;

    int temp, rc;
    while ((rc = fscanf(file, "%i ", &temp)) == 1)
        elements_count++;

    return rc != EOF ? -1 : elements_count;
}

void read_elements_from_file(FILE *file, int **begin, int **end, int elements_count)
{
    assert(file != NULL);
    assert(begin != NULL);
    assert(end != NULL);
    assert(*begin != NULL);
    assert(elements_count > 0);

    rewind(file);
    *end = *begin;
    while (*end != *begin + elements_count)
        fscanf(file, "%i", (*end)++);
}

int read_data_file(char *filename, int **begin, int **end)
{
    assert(filename != NULL);
    assert(begin != NULL);
    assert(end != NULL);

    int status_code = SUCCESS;

    FILE *file = fopen(filename, "rt");
    if (file == NULL)
        return CANT_OPEN_FILE;

    int elements_count = count_elements_in_file(file);
    if (elements_count <= 0)
    {
        status_code = INVALID_ELEMENTS_AMOUNT;
    }
    else
    {
        *begin = (int *)malloc(elements_count * sizeof(int));
        if (*begin == NULL)
        {
            status_code = BAD_ALLOC;
        }
        else
        {
            // actually read data from file (cant raise exceptions)
            read_elements_from_file(file, begin, end, elements_count);
        }
    }

    fclose(file);
    return status_code;
}

int parse_args(int argc, char **argv, char **input_filename, char **output_filename, bool *need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);
    assert(need_filtration != NULL);

    if (argc != 3 && argc != 4 && argc != 7)
        return INVALID_ARGS;

    *input_filename = argv[1];
    *output_filename = argv[2];

    *need_filtration = false;
    if (argc == 4)
    {
        if (strcmp(argv[3], FILTER_OPT_STR) == 0)
            *need_filtration = true;
        else
            return INVALID_OPT; // unrecognized option
    }

    return SUCCESS;
}

int do_tasks(int **begin, int **end, bool need_filtration)
{
    int status_code = SUCCESS;

    if (need_filtration)
    {
        int *filtered_begin;
        int *filtered_end;

        status_code = key(*begin, *end, &filtered_begin, &filtered_end);
        if (status_code == SUCCESS)
        {
            free(*begin);
            *begin = filtered_begin;
            *end = filtered_end;
        }
    }

    if (status_code == SUCCESS)
        status_code = mysort(*begin, *end - *begin, sizeof(int), int_comparator);

    return status_code;
}

int proccess(char *input_filename, char *output_filename, bool need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);

    int *data_array_begin = NULL;
    int *data_array_end = NULL;
    int status_code = SUCCESS;

    if (read_data_file(input_filename, &data_array_begin, &data_array_end))
        status_code = FAILURE;
    else
    {
        status_code = do_tasks(&data_array_begin, &data_array_end, need_filtration);

        if (status_code == SUCCESS)
            status_code = write_file(output_filename, data_array_begin, data_array_end);

        free(data_array_begin);
    }

    return status_code;
}

int main(int argc, char **argv)
{
    char *input_filename = NULL;
    char *output_filename = NULL;

    bool need_filtration = false;

    if (parse_args(argc, argv, &input_filename, &output_filename, &need_filtration))
        return FAILURE;

    return proccess(input_filename, output_filename, need_filtration);
}
