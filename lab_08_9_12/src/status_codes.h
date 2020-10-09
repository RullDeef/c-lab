#ifndef __STATUS_CODES_H_
#define __STATUS_CODES_H_

typedef enum
{
    success = 0,
    failure,
    invalid_app_params,
    invalid_filename,
    invalid_file,
    invalid_command,
    invalid_mat_dims
} status_code_t;

#endif // __STATUS_CODES_H_
