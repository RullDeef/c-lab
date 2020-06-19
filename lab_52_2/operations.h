#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include "status_codes.h"

status_code_t sort_products(char *filename_in, char *filename_out);
status_code_t find_products(char *filename_in, char *substr);

#endif
