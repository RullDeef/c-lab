#include "commands.h"


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

