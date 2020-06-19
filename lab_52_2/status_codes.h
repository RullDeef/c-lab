#ifndef _STATUS_CODES_H_
#define _STATUS_CODES_H_

typedef enum
{
    exit_success = 0,
    exit_failure,
    end_of_file,
    empty_file,
    too_many_products,
    cant_read_line,
    cant_read_product,
    cant_write_product,
    cant_open_input_file,
    cant_open_output_file,
    cant_find_products,
    unsupported_args = 53
} status_code_t;

#endif
