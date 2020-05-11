#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    exit_success = 0,
    invalid_argc,
    file_not_found,
    invalid_file_format
} status_code_t;

status_code_t extract_file_name(int argc, char *argv[], char **fname);
status_code_t process(FILE *file, size_t *numbers_amount);

int main(int argc, char *argv[])
{
    char *fname;
    FILE *file;

    size_t numbers_amount;
    status_code_t status_code;

    status_code = extract_file_name(argc, argv, &fname);

    if (status_code == exit_success)
    {
        file = fopen(fname, "r");

        if (file == NULL)
            status_code = file_not_found;
        else
        {
            status_code = process(file, &numbers_amount);

            if (status_code == exit_success)
                fprintf(stdout, "%zu", numbers_amount);
            
            fclose(file);
        }
    }

    return status_code;
}

status_code_t extract_file_name(int argc, char *argv[], char **fname)
{
    *fname = NULL;
    status_code_t status_code = exit_success;

    if (argc != 2)
        status_code = invalid_argc;
    else
        *fname = argv[1];

    return status_code;
}

status_code_t process(FILE *file, size_t *numbers_amount)
{
    float current_number;
    float max_number;
    float min_number;
    bool first_number_readed = true;

    float mean;
    *numbers_amount = 0;
    status_code_t status_code = exit_success;

    int scan_status;
    while ((scan_status = fscanf(file, "%f", &current_number)) == 1)
    {
        if (first_number_readed)
        {
            max_number = current_number;
            min_number = current_number;
            first_number_readed = false;
        }
        else
        {
            max_number = current_number > max_number ? current_number : max_number;
            min_number = current_number < min_number ? current_number : min_number;
        }
    }

    if (scan_status != EOF || first_number_readed)
        status_code = invalid_file_format;
    else
    {
        rewind(file);
        mean = (min_number + max_number) / 2;

        while (fscanf(file, "%f", &current_number) == 1)
            if (current_number > mean)
                (*numbers_amount)++;
    }

    return status_code;
}