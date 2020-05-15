#include <stdio.h>
#include "defs.h"

status_code_t parse_command(int argc, char *argv[], command_t *command, fmode_t *mode);
status_code_t command_sort_func(const fmode_t mode, const char *fname);
status_code_t command_find_func(const fmode_t mode, const char *fin_name, const char *fout_name, const char *substr);
status_code_t command_delete_func(const fmode_t mode, const char *fname);

status_code_t command_sort_func_text(FILE *file);
status_code_t command_sort_func_binary(FILE *file);
status_code_t command_find_func_text(FILE *file_in, FILE *file_out, const char *substr);
status_code_t command_find_func_binary(FILE *file_in, FILE *file_out, const char *substr);
status_code_t command_delete_func_text(FILE *file);
status_code_t command_delete_func_binary(FILE *file);
