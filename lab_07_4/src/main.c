#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "status_codes.h"
#include "filter_func.h"
#include "sort_func.h"

#define FILTER_OPT_STR "f"

void print_array_to_file(FILE *file, int *begin, int *end)
{
    fprintf(file, "%d", *begin);
    for (begin++; begin != end; begin++)
        fprintf(file, " %d", *begin);
}

int write_file(char *filename, int *begin, int *end)
{
    assert(filename != NULL);
    assert(begin != NULL);
    assert(end != NULL);
    assert(begin < end);

    FILE *file = fopen(filename, "wt");
    if (file == NULL)
        return cant_open_file;

    print_array_to_file(file, begin, end);
    fclose(file);
    return success;
}

int count_elements_in_file(FILE *file)
{
    assert(file != NULL);

    int elements_count = 0;

    int temp, rc;
    while ((rc = fscanf(file, "%i ", &temp)) == 1)
        elements_count++;

    return rc == EOF ? elements_count : failure;
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

    FILE *file = fopen(filename, "rt");
    if (file == NULL)
        return cant_open_file;

    int status_code = success;

    int elements_count = count_elements_in_file(file);
    if (elements_count > 0)
    {
        *begin = (int *)malloc(elements_count * sizeof(int));
        if (*begin != NULL)
            read_elements_from_file(file, begin, end, elements_count);
        else
            status_code = bad_alloc;
    }
    else
        status_code = invalid_elements_amount;

    fclose(file);
    return status_code;
}

int parse_args(int argc, char **argv, char **input_filename, char **output_filename, bool *need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);
    assert(need_filtration != NULL);

    if (argc != 3 && argc != 4 && argc != 7)
        return invalid_args;

    int status_code = success;

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
        status_code = mysort(*begin, *end - *begin, sizeof(int), int_comparator);

    return status_code;
}

int proccess(char *input_filename, char *output_filename, bool need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);

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
