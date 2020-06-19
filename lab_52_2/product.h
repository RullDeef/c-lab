#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include <stdbool.h>
#include <inttypes.h>
#include "status_codes.h"

#define MAX_NAME_SIZE 31
#define MAX_MFR_SIZE  16

typedef struct
{
    char name[MAX_NAME_SIZE];
    char mfr[MAX_MFR_SIZE];
    uint32_t price;
    uint32_t amount;
} product_t;

status_code_t product_read(FILE *file, product_t *product);
status_code_t product_write(FILE *file, product_t *product);

bool product_equal(product_t *product_1, product_t *product_2);
bool product_ordered(product_t *product_1, product_t *product_2);
void product_swap(product_t *product_1, product_t *product_2);

#endif
