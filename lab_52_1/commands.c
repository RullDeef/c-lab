#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "student.h"
#include "commands.h"

status_code_t parse_command(int argc, char *argv[], command_t *command)
{
    status_code_t status_code = exit_success;

    if (argc == 3)
    {
        if (strcmp(argv[1], "sb") == 0)
            *command = command_sort;
        else if (strcmp(argv[1], "db") == 0)
            *command = command_delete;
        else
            status_code = invalid_command;
    }
    else if (argc == 5)
    {
        if (strcmp(argv[1], "fb") == 0)
            *command = command_find;
        else
            status_code = invalid_command;
    }
    else
        status_code = invalid_args_amount;

    return status_code;
}

status_code_t command_sort_func(const char *fname)
{
    FILE *file;
    status_code_t status_code = exit_success;

    file = fopen(fname, "r+b");

    if (file == NULL)
        status_code = invalid_file_name;
    else
    {
        status_code = command_sort_func_binary(file);
        fclose(file);
    }

    return status_code;
}

status_code_t command_find_func(const char *fin_name, const char *fout_name, const char *substr)
{
    FILE *file_in;
    FILE *file_out;
    status_code_t status_code = exit_success;

    if (strcmp(fin_name, fout_name) == 0)
        status_code = same_input_output_file;
    else
    {
        file_in = fopen(fin_name, "rb");

        if (file_in == NULL)
            status_code = invalid_file_name;
        else
        {
            file_out = fopen(fout_name, "wb");

            if (file_out == NULL)
                status_code = invalid_file_name;
            else
            {
                status_code = command_find_func_binary(file_in, file_out, substr);
                fclose(file_out);
            }

            fclose(file_in);
        }
    }

    return status_code;
}

status_code_t command_delete_func(const char *fname)
{
    FILE *file;
    status_code_t status_code = exit_success;

    file = fopen(fname, "r+b");

    if (file == NULL)
        status_code = invalid_file_name;
    else
    {
        status_code = command_delete_func_binary(file);
        fclose(file);
    }

    return status_code;
}

status_code_t validate_file_size(FILE* file, size_t* size)
{
    fseek(file, 0L, SEEK_END);
    *size = ftell(file);

    if (*size == 0)
        return end_of_file;
    else if (*size / sizeof(student_t) * sizeof(student_t) != *size)
        return invalid_file_format;
    else
        return exit_success;
}

status_code_t command_sort_func_binary(FILE *file)
{
    student_t student_i;
    student_t student_j;
    size_t size;

    status_code_t status_code = validate_file_size(file, &size);

    if (status_code == exit_success)
    {
        size /= sizeof(student_t);
        for (size_t i = 0; i + 1 < size; i++)
        {
            student_read_binary(file, i, &student_i);
            for (size_t j = i + 1; j < size; j++)
            {
                student_read_binary(file, j, &student_j);

                if (student_compare((const void *)&student_i, (const void *)&student_j) > 0)
                {
                    student_write_binary(file, i, &student_j);
                    student_write_binary(file, j, &student_i);
                    //student_i = student_j;
                }
            }
        }

        for (size_t i = 0; i < size; i++)
        {
            student_read_binary(file, i, &student_i);
            fprintf(stdout, "%s %s", student_i.surname, student_i.first_name);
            for (size_t m = 0; m < MARKS_AMOUNT; m++)
                fprintf(stdout, " %u", student_i.marks[m]);
            fprintf(stdout, "\n");
        }
    }

    return status_code;
}

status_code_t command_find_func_binary(FILE *file_in, FILE *file_out, const char *substr)
{
    student_t student;
    size_t size;
    size_t j = 0;

    status_code_t status_code = validate_file_size(file_in, &size);

    if (status_code == exit_success)
    {
        size /= sizeof(student_t);
        for (size_t i = 0; i < size; i++)
        {
            student_read_binary(file_in, i, &student);
            if (student_startswith(&student, substr))
            {
                ftruncate(fileno(file_out), (j + 1) * sizeof(student_t));
                student_write_binary(file_out, j, &student);
                j++;
            }
        }
        if (j == 0)
            status_code = no_students_found;
    }

    return status_code;
}

float get_mean_of_the_mean(FILE *file)
{
    student_t student;
    size_t size = 0;
    float mean = 0.0f;

    fseek(file, 0L, SEEK_SET);
    while (fread(&student, sizeof(student_t), 1, file))
    {
        mean += student_mean(&student);
        size++;
    }

    return mean / size;
}

status_code_t command_delete_func_binary(FILE *file)
{
    student_t student_i;
    student_t student_j;
    size_t size;

    status_code_t status_code = validate_file_size(file, &size);

    if (status_code == exit_success)
    {
        size /= sizeof(student_t);
        float mean = get_mean_of_the_mean(file);
        for (size_t i = size - 1; i >= 0; i--)
        {
            student_read_binary(file, i, &student_i);
            if (student_mean(&student_i) < mean)
            {
                for (size_t j = i; j + 1 < size; j++)
                {
                    student_read_binary(file, j + 1, &student_j);
                    student_write_binary(file, j, &student_j);
                }
                size--;
            }
            if (i == 0)
                break;
        }
        
        ftruncate(fileno(file), size * sizeof(student_t));
    }

    return status_code;
}
