#include <stdio.h>
#include "defs.h"

status_code_t parse_command(int argc, char *argv[], command_t *command, mode_t *mode);
status_code_t command_sort_func(const mode_t mode, char *fname);
status_code_t command_find_func(const mode_t mode, char *fin_name, char *fout_name, char *substr);
status_code_t command_delete_func(const mode_t mode, char *fname);

status_code_t command_sort_func_text(FILE *file);
status_code_t command_sort_func_binary(FILE *file);
status_code_t command_find_func_text(FILE *file);
status_code_t command_find_func_binary(FILE *file);
status_code_t command_delete_func_text(FILE *file);
status_code_t command_delete_func_binary(FILE *file);
