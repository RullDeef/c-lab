#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "filter_func.h"
#include "sort_func.h"
#include <assert.h>

#define FILTER_OPT_STR "f"


int write_file(char *filename, int *data_array_begin, int *data_array_end)
{
    assert(filename != NULL);
    assert(data_array_begin != NULL);
    assert(data_array_end != NULL);

    FILE *file = fopen(filename, "wt");
    if (file == NULL)
        return -1;

    for (int *iter = data_array_begin; iter != data_array_end; iter++)
        fprintf(file, "%i\n", *iter);

    fclose(file);
    return 0;
}

int count_elements_in_file(FILE *file)
{
    assert(file != NULL);

    int elements_count = 0;

    int temp, rc;
    while ((rc = fscanf(file, "%i ", &temp)) == 1)
        elements_count++;

    if (rc != EOF)
        return -2;

    return elements_count;
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
        return -1;

    int elements_count = count_elements_in_file(file);
    if (elements_count <= 0)
    {
        fclose(file);
        return -2;
    }

    *begin = (int *)malloc(elements_count * sizeof(int));
    if (*begin == NULL)
    {
        fclose(file);
        return -3;
    }

    // actually read data from file (cant raise exceptions)
    read_elements_from_file(file, begin, end, elements_count);

    fclose(file);
    return 0;
}

int parse_args(int argc, char **argv, char **input_filename, char **output_filename, bool *need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);
    assert(need_filtration != NULL);

    if (argc != 3 && argc != 4 && argc != 7)
        return -1;

    *input_filename = argv[1];
    *output_filename = argv[2];

    *need_filtration = false;
    if (argc == 4)
    {
        if (strcmp(argv[3], FILTER_OPT_STR) == 0)
            *need_filtration = true;
        else
            return -2; // unrecognized option
    }

    return 0;
}

int apply_filtration(int **begin, int **end)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(*begin != NULL);
    assert(*end != NULL);
    assert(*begin <= *end);

    if (*begin == *end)
        return -1;

    if (key(*begin, *end, begin, end))
        return -2;

    return 0;
}

int do_tasks(int *begin, int **end, bool need_filtration)
{
    int status_code = 0;

    if (need_filtration)
        status_code = apply_filtration(&begin, end);

    return status_code || mysort(begin, *end - begin, sizeof(int), int_comparator, int_swapper);
}

int proccess(char *input_filename, char *output_filename, bool need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);

    int *data_array_begin = NULL;
    int *data_array_end = NULL;
    int status_code = 0;

    if (read_data_file(input_filename, &data_array_begin, &data_array_end))
        status_code = -1;
    else
    {
        status_code = do_tasks(data_array_begin, &data_array_end, need_filtration)
            || write_file(output_filename, data_array_begin, data_array_end);

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
        return -1;

    return proccess(input_filename, output_filename, need_filtration);
}
