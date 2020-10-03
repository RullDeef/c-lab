#ifndef _STATUS_CODES_H_
#define _STATUS_CODES_H_

typedef enum
{
    success,
    failure,
    too_few_args,
    too_many_args,
    invalid_args,
    cant_open_file,
    cant_read_items,
    cant_process_items,
    max_items_amount_reached,
    max_item_name_size_reached,
    invalid_item_name,
    invalid_item_volume,
    invalid_item,
    end_of_file_reached
} status_code_t;

#endif // _STATUS_CODES_H_
