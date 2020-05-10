#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

typedef int32_t number_t;

typedef enum
{
    exit_success = 0,
    invalid_args_count,
    invalid_command,
    invalid_file_name,
    invalid_file_format
} status_code_t;

typedef enum
{
    create_file_command,
    print_file_command,
    sort_file_command
} command_t;

status_code_t parse_command_type(int argc, char *argv[], command_t *command);
number_t get_number_by_pos(FILE *file, const size_t pos);
void set_number_by_pos(FILE *file, const size_t pos, const number_t number);
status_code_t check_file_format(FILE *file);
status_code_t create_file(char *argv[]);
status_code_t print_file(const char *fname);
status_code_t sort_file(const char *fname);

int main(int argc, char *argv[])
{
    command_t command;
    status_code_t status_code = exit_success;

    status_code = parse_command_type(argc, argv, &command);

    if (status_code == exit_success)
    {
        switch (command)
        {
            case create_file_command:
                status_code = create_file(argv);
                break;
            case print_file_command:
                status_code = print_file(argv[2]);
                break;
            case sort_file_command:
                status_code = sort_file(argv[2]);
                break;
        }
    }

    return status_code;
}

status_code_t parse_command_type(int argc, char *argv[], command_t *command)
{
    status_code_t status_code = exit_success;

    if (argc == 4)
    {
        if (strcmp(argv[1], "c") * strcmp(argv[2], "c") * strcmp(argv[3], "c") == 0)
            *command = create_file_command;
        else
            status_code = invalid_command;
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "p") == 0)
            *command = print_file_command;
        else if (strcmp(argv[1], "s") == 0)
            *command = sort_file_command;
        else
            status_code = invalid_command;
    }
    else
        status_code = invalid_args_count;

    return status_code;
}

number_t get_number_by_pos(FILE *file, const size_t pos)
{
    number_t number;

    fseek(file, sizeof(number_t) * pos, SEEK_SET);
    fread(&number, sizeof(number_t), 1, file);

    return number;
}

void set_number_by_pos(FILE *file, const size_t pos, const number_t number)
{
    fseek(file, sizeof(number_t) * pos, SEEK_SET);
    fwrite(&number, sizeof(number_t), 1, file);
}

status_code_t extract_file_name(char *argv[], char **fname)
{
    status_code_t status_code = exit_success;

    *fname = argv[2]; // TODO

    return status_code;
}

status_code_t extract_file_size(char *argv[], char *fname, size_t *size)
{
    status_code_t status_code = exit_success;

    char *c;
    *size = strtol(argv[3], &c, 10); // TODO

    return status_code;
}

status_code_t check_file_format(FILE *file)
{
    size_t actual_size;
    status_code_t status_code = exit_success;

    fseek(file, 0, SEEK_END);
    actual_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (actual_size / sizeof(number_t) * sizeof(number_t) != actual_size)
        status_code = invalid_file_format;

    return status_code;
}

status_code_t create_file(char *argv[])
{
    char *fname;
    size_t size;
    FILE *file;
    number_t number;
    status_code_t status_code = exit_success;

    status_code = extract_file_name(argv, &fname);
    if (status_code == exit_success)
    {
        status_code = extract_file_size(argv, fname, &size);
        if (status_code == exit_success)
        {
            file = fopen(fname, "wb");

            if (file == NULL)
                status_code = invalid_file_name;
            else
            {
                for (size_t i = 0; i < size; i++)
                {
                    number = rand() % 200 - 100;
                    fwrite(&number, sizeof(number_t), 1, file);
                }

                fclose(file);
            }
        }
    }

    return status_code;
}

status_code_t print_file(const char *fname)
{
    number_t number;
    status_code_t status_code = exit_success;

    FILE *file = fopen(fname, "r+b");

    if (file == NULL)
        status_code = invalid_file_name;
    else
    {
        status_code = check_file_format(file);

        if (status_code == exit_success)
        {
            size_t printed_count = 0;
            while (fread(&number, sizeof(number_t), 1, file) == 1)
            {
                if (printed_count == 0)
                    fprintf(stdout, "%d", number);
                else
                    fprintf(stdout, " %d", number);
                printed_count++;
            }
        }
        
        fclose(file);
    }

    return status_code;
}

status_code_t sort_file(const char *fname)
{
    FILE *file;
    size_t size;
    number_t number_i;
    number_t number_j;

    status_code_t status_code = exit_success;

    file = fopen(fname, "r+b");

    if (file == NULL)
        status_code = invalid_file_name;
    else
    {
        status_code = check_file_format(file);

        if (status_code == exit_success)
        {
            fseek(file, 0, SEEK_END);
            size = ftell(file) / sizeof(number_t);

            for (size_t i = 0; i + 1 < size; i++)
            {
                for (size_t j = i + 1; j < size; j++)
                {
                    number_i = get_number_by_pos(file, i);
                    number_j = get_number_by_pos(file, j);
                    if (number_i > number_j)
                    {
                        set_number_by_pos(file, i, number_j);
                        set_number_by_pos(file, j, number_i);
                    }
                }
            }
        }

        fclose(file);
    }

    return status_code;
}
