#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_params.h"

#define ADD_COMMAND_STRING "a"
#define MULT_COMMAND_STRING "m"
#define SSLE_COMMAND_STRING "o"

typedef enum
{
    success = 0,
    invalid_command,
    too_large_filename
} status_code_t; // local status codes


inline static int imp__str_equal(const char *str_1, const char *str_2)
{
    return strcmp(str_1, str_2) == 0;
}

static status_code_t imp__parse_app_command(const char *str, app_command_t *command)
{
    status_code_t status_code = success;

    if (imp__str_equal(str, ADD_COMMAND_STRING))
        *command = command_add;
    else if (imp__str_equal(str, MULT_COMMAND_STRING))
        *command = command_mult;
    else if (imp__str_equal(str, SSLE_COMMAND_STRING))
        *command = command_ssle;
    else
        status_code = invalid_command;

    return status_code;
}

static status_code_t imp__parse_filename(const char *str, filename_t filename)
{
    status_code_t status_code = success;

    if (strlen(str) >= MAX_FILENAME_LENGTH)
        status_code = too_large_filename;
    else
        strcpy(filename, str);

    return status_code;
}

app_params_t *parse_app_params(int argc, const char **argv)
{
    app_params_t *app_params = NULL;

    if (argc == 4 || argc == 5)
    {
        app_params = malloc(sizeof(app_params_t));
        if (app_params)
        {
            if (imp__parse_app_command(argv[1], &app_params->command) != success)
            {
                free(app_params);
                app_params = NULL;
            }
            else if (app_params->command == command_ssle && argc == 4)
            {
                if (imp__parse_filename(argv[2], app_params->ifname_1) != success ||
                    imp__parse_filename(argv[3], app_params->ofname) != success)
                {
                    free(app_params);
                    app_params = NULL;
                }
            }
            else if (app_params->command != command_ssle && argc == 5)
            {
                if (imp__parse_filename(argv[2], app_params->ifname_1) != success ||
                    imp__parse_filename(argv[3], app_params->ifname_2) != success ||
                    imp__parse_filename(argv[4], app_params->ofname) != success)
                {
                    free(app_params);
                    app_params = NULL;
                }
            }
            else
            {
                free(app_params);
                app_params = NULL;
            }
        }
    }

    return app_params;
}
