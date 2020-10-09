#ifndef __APP_PARAMS_H_
#define __APP_PARAMS_H_

#define MAX_FILENAME_LENGTH 128

typedef enum
{
    command_add,
    command_mult,
    command_ssle
} app_command_t;

typedef char filename_t[MAX_FILENAME_LENGTH];

typedef struct
{
    app_command_t command;
    filename_t ifname_1;
    filename_t ifname_2;
    filename_t ofname;
} app_params_t;

app_params_t *parse_app_params(int argc, const char **argv);

#endif // __APP_PARAMS_H_
