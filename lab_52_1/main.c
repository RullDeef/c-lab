#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <inttypes.h>

#define MAX_SURNAME_LENGTH 25
#define MAX_FIRST_NAME_LENGTH 10
#define MARKS_AMOUNT 4

typedef enum
{
    exit_success = 0,
    invalid_command_line_args_amount,
    invalid_command_type_or_mode,
    invalid_file_name,
    invalid_command_line_args = 53
} status_code_t;

typedef enum
{
    command_sort = 0,
    command_find,
    command_delete
} command_t;

typedef enum
{
    mode_text = 0,
    mode_binary
} mode_t;

typedef struct
{
    char surname[MAX_SURNAME_LENGTH + 1],
    char first_name[MAX_FIRST_NAME_LENGTH + 1],
    int32_t marks[MARKS_AMOUNT]
} student_t;

status_code_t parse_command(int argc, char *argv[], command_t *command, mode_t *mode);
status_code_t command_sort_func(const mode_t mode, char *fname);
status_code_t command_find_func(const mode_t mode, char *fin_name, char *fout_name, char *substr);
status_code_t command_delete_func(const mode_t mode, char *fname);

int main(int argc, char *argv[])
{
    command_t command;
    mode_t mode;
    status_code_t status_code = exit_success;

    status_code = parse_command(argc, argv, &command, &mode);

    if (status_code == exit_success)
    {
        switch (command)
        {
            case command_sort:
                status_code = command_sort_func(mode, argv[2]);
                break;
            
            case command_find:
                status_code = command_find_func(mode, argv[2], argv[3], argv[4]);
                break;
            
            case command_delete:
                status_code = command_delete_func(mode, argv[2]);
                break;
        }
    }
    else // to be sure we return 53 on any unrecognized command line arguments
        status_code = invalid_command_line_args;

    return status_code;
}

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
