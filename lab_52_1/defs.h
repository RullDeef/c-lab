#ifndef __DEFS_H
#define __DEFS_H

typedef enum
{
    exit_success = 0,
    invalid_command_line_args_amount,
    invalid_command_type_or_mode,
    invalid_file_name,
    invalid_file_format,
    end_of_file,
    empty_surname,
    empty_first_name,
    too_long_surname,
    too_long_first_name,
    invalid_marks_line,
    invalid_marks_amount,
    invalid_mark_domain,
    too_many_students,
    same_input_output_file,
    same_students,
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

#endif
