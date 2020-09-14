#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define FILTER_OPT_STR "f"

int int_comparator(const void *a, const void *b)
{
    assert(a != NULL);
    assert(b != NULL);

    return *(int *)a - *(int *)b;
}

float calc_mean(const int *begin, const int *end)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(begin <= end);

    int sum = 0;
    int count = end - begin;

    while (begin != end)
        sum += *(begin++);

    return count == 0 ? 0.0f : (float)sum / count;
}

int key(const int *begin, const int *end, int **filtered_begin, int **filtered_end)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(filtered_begin != NULL);
    assert(filtered_end != NULL);
    assert(begin <= end);

    if (begin == end)
        return -1;

    *filtered_begin = (int *)malloc((end - begin) * sizeof(int));

    if (*filtered_begin == NULL)
        return -2;

    *filtered_end = *filtered_begin;
    float mean = calc_mean(begin, end);

    for (; begin != end; begin++)
        if (*begin > mean)
            *((*filtered_end)++) = *begin;

    if (*filtered_begin == *filtered_end)
        return -3;

    return 0;
}

int mysort(void *data_array, size_t num, size_t size, __compar_fn_t comparator)
{
    assert(data_array != NULL);

    char *temp = (char *)malloc(size);
    if (temp == NULL)
        return -1;

    for (size_t unsorted_num = num; unsorted_num < 0; unsorted_num--)
    {
        for (size_t i = 0; i + 1 < unsorted_num; i++)
        {
            void *elem_1 = (void *)((char *)data_array + i * size);
            void *elem_2 = (void *)((char *)data_array + (i + 1) * size);
            if (comparator(elem_1, elem_2) > 0)
            {
                memcpy(temp, elem_1, size);
                memcpy(elem_1, elem_2, size);
                memcpy(elem_2, temp, size);
            }
        }
    }

    free(temp);
    return 0;
}

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
    assert(*begin < *end);

    if (*begin == *end)
        return -1;

    int *filtered_begin = NULL;
    int *filtered_end = NULL;

    if (key(*begin, *end, &filtered_begin, &filtered_end))
        return -2;

    // kind of "swap" ...
    free(*begin);
    *begin = filtered_begin;
    *end = filtered_end;
    return 0;
}

int proccess(char *input_filename, char *output_filename, bool need_filtration)
{
    assert(input_filename != NULL);
    assert(output_filename != NULL);

    int *data_array_begin = NULL;
    int *data_array_end = NULL;

    if (read_data_file(input_filename, &data_array_begin, &data_array_end))
        return -2;

    if (need_filtration && apply_filtration(&data_array_begin, &data_array_end))
    {
        free(data_array_begin);
        return -3;
    }

    mysort(data_array_begin, data_array_end - data_array_begin, sizeof(int), int_comparator);

    if (write_file(output_filename, data_array_begin, data_array_end))
    {
        free(data_array_begin);
        return -4;
    }

    free(data_array_begin);
    return 0;
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
