#ifndef STATUS_CODES_H
#define STATUS_CODES_H

enum status_codes
{
    success = 0,
    failure = -1,
    invalid_args = -2,
    invalid_opt = -3,
    invalid_ptrs = -4,
    bad_alloc = -5,
    cant_open_file = -6,
    invalid_elements_amount = -7,
    invalid_elements_size = -8
};

#endif // STATUS_CODES_H
