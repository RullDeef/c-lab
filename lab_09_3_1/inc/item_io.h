#ifndef __ITEM_IO_H_
#define __ITEM_IO_H_

#include <stdio.h>
#include "item.h"
#include "item_array.h"

struct item it_read_file(FILE *file);
void it_print_file(FILE *file, const struct item *it);

struct item_array it_read_array_file(FILE *file);
void it_print_array_file_constraint(FILE *file, const struct item_array *ita, float price);

struct item_array it_read_array(const char *fname);

#endif // __ITEM_IO_H_
