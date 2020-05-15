#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"
#include "student.h"
#include "commands.h"

status_code_t parse_command(int argc, char *argv[], command_t *command, fmode_t *mode)
{
    status_code_t status_code = exit_success;

    if (argc == 3)
    {
        /*
        if (strcmp(argv[1], "st") == 0)
        {
            *command = command_sort;
            *mode = mode_text;
        }
        else
        */
        if (strcmp(argv[1], "sb") == 0)
        {
            *command = command_sort;
            *mode = mode_binary_read_write;
        }
        /*
        else if (strcmp(argv[1], "dt") == 0)
        {
            *command = command_delete;
            *mode = mode_text;
        }
        */
        else if (strcmp(argv[1], "db") == 0)
        {
            *command = command_delete;
            *mode = mode_binary_read_write;
        }
        else
            status_code = invalid_command_type_or_mode;
    }
    else if (argc == 5)
    {
        /*
        if (strcmp(argv[1], "ft") == 0)
        {
            *command = command_find;
            *mode = mode_text;
        }
        else
        */
        if (strcmp(argv[1], "fb") == 0)
        {
            *command = command_find;
            *mode = mode_binary_read;
        }
        else
            status_code = invalid_command_type_or_mode;
    }
    else
        status_code = invalid_command_line_args_amount;

    return status_code;
}

FILE *fopen_with_mode(const char *fname, fmode_t mode)
{
    FILE *file;

    switch (mode)
    {
        case mode_binary_read:
            file = fopen(fname, "rb");
            break;
        case mode_binary_write:
            file = fopen(fname, "wb");
            break;
        case mode_binary_read_write:
            file = fopen(fname, "r+b");
            break;
    }

    return file;
}

status_code_t command_sort_func(const fmode_t mode, const char *fname)
{
    FILE *file;
    status_code_t status_code = exit_success;

    file = fopen_with_mode(fname, mode_binary_read_write);

    status_code = command_sort_func_binary(file);
    fclose(file);

    return status_code;
}

status_code_t command_find_func(const fmode_t mode, const char *fin_name, const char *fout_name, const char *substr)
{
    FILE *file_in;
    FILE *file_out;
    status_code_t status_code = exit_success;

    if (strcmp(fin_name, fout_name) == 0)
        status_code = same_input_output_file;
    else
    {
        file_in = fopen_with_mode(fin_name, mode_binary_read);

        if (file_in == NULL)
            status_code = invalid_file_name;
        else
        {
            file_out = fopen_with_mode(fout_name, mode_binary_write);

            status_code = command_find_func_binary(file_in, file_out, substr);
            fclose(file_out);
            fclose(file_in);
        }
    }

    return status_code;
}

status_code_t command_delete_func(const fmode_t mode, const char *fname)
{
    FILE *file;
    status_code_t status_code = exit_success;

    file = fopen_with_mode(fname, mode);

    status_code = command_delete_func_binary(file);
    fclose(file);

    return status_code;
}

/*
status_code_t command_sort_func_text(FILE *file)
{
    status_code_t status_code = exit_success;

    #define MAX_STUDENTS_AMOUNT 100
    student_t students[MAX_STUDENTS_AMOUNT];
    size_t students_amount = 0;

    do
    {
        if (students_amount >= MAX_STUDENTS_AMOUNT)
        {
            status_code = too_many_students;
            break;
        }
        
        status_code = student_read_text(file, &students[students_amount]);
        if (status_code != exit_success)
            break;
        
        students_amount++;
    } while (status_code == exit_success);

    qsort(students, students_amount, sizeof(student_t), student_compare);

    for (size_t i = 0; i < students_amount; i++)
        student_write_text(stdout, &students[i]);

    return status_code;
}
*/

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
                student_write_binary(file_out, j, &student);
                j++;
            }
        }
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
