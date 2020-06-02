#ifndef __DEFS_H
#define __DEFS_H

typedef enum
{
    exit_success = 0,
    invalid_args_amount,
    invalid_command,
    invalid_file_name,
    invalid_file_format,
    end_of_file,
    same_input_output_file,
    no_students_found,
    invalid_command_line_args = 53
} status_code_t;

typedef enum
{
    command_sort = 0,
    command_find,
    command_delete
} command_t;

#endif
