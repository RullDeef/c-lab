#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "student.h"
#include "commands.h"

status_code_t parse_command(int argc, char *argv[], command_t *command, mode_t *mode)
{
    status_code_t status_code = exit_success;

    if (argc == 3)
    {
        if (strcmp(argv[1], "st") == 0)
        {
            *command = command_sort;
            *mode = mode_text;
        }
        else if (strcmp(argv[1], "sb") == 0)
        {
            *command = command_sort;
            *mode = mode_binary;
        }
        else if (strcmp(argv[1], "dt") == 0)
        {
            *command = command_delete;
            *mode = mode_text;
        }
        else if (strcmp(argv[1], "db") == 0)
        {
            *command = command_delete;
            *mode = mode_binary;
        }
        else
            status_code = invalid_command_type_or_mode;
    }
    else if (argc == 5)
    {
        if (strcmp(argv[1], "ft") == 0)
        {
            *command = command_find;
            *mode = mode_text;
        }
        else if (strcmp(argv[1], "fb") == 0)
        {
            *command = command_find;
            *mode = mode_binary;
        }
        else
            status_code = invalid_command_type_or_mode;
    }
    else
        status_code = invalid_command_line_args_amount;

    return status_code;
}

FILE *fopen_with_mode(const char *fname, mode_t mode)
{
    FILE *file;

    switch (mode)
    {
        case mode_text:
            file = fopen(fname, "r+t");
            break;
        case mode_binary:
            file = fopen(fname, "r+b");
            break;
    }

    return file;
}

status_code_t command_sort_func(const mode_t mode, char *fname)
{
    FILE *file;
    status_code_t status_code = exit_success;

    file = fopen_with_mode(fname, mode);

    if (file == NULL)
        status_code = invalid_file_name;
    else
    {
        switch (mode)
        {
        case mode_text:
            status_code = command_sort_func_text(file);
            break;

        case mode_binary:
            status_code = command_sort_func_binary(file);
            break;
        }

        fclose(file);
    }

    return status_code;
}

status_code_t command_find_func(const mode_t mode, char *fin_name, char *fout_name, char *substr)
{
    FILE *file_in;
    FILE *file_out;
    status_code_t status_code = exit_success;

    // what if open the same file twice?
    // 2000 years later

    return status_code;
}

status_code_t command_delete_func(const mode_t mode, char *fname)
{
    FILE *file;
    status_code_t status_code = exit_success;

    file = fopen_with_mode(fname, mode);

    if (file == NULL)
        status_code = invalid_file_name;
    else
    {
        switch (mode)
        {
        case mode_text:
            status_code = command_delete_func_text(file);
            break;

        case mode_binary:
            status_code = command_delete_func_binary(file);
            break;
        }

        fclose(file);
    }

    return status_code;
}

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

status_code_t command_sort_func_binary(FILE *file)
{
    status_code_t status_code = exit_success;

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);

    if (size / sizeof(student_t) * sizeof(student_t) != size)
        status_code = invalid_file_format;
    else
    {
        size /= sizeof(student_t);
        for (size_t i = 0; i < size; i++)
        {
            student_t student;
            fread(&student, sizeof(student_t), 1, file);
            // ...
        }
    }

    return status_code;
}

status_code_t command_find_func_text(FILE *file)
{
    status_code_t status_code = exit_success;
    return status_code;
}

status_code_t command_find_func_binary(FILE *file)
{
    status_code_t status_code = exit_success;
    return status_code;
}

status_code_t command_delete_func_text(FILE *file)
{
    status_code_t status_code = exit_success;
    return status_code;
}

status_code_t command_delete_func_binary(FILE *file)
{
    status_code_t status_code = exit_success;
    return status_code;
}
