#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include "product.h"
#include "operations.h"

status_code_t read_line(char* dest, size_t size, FILE *file)
{
    assert(dest != NULL && file != NULL);

    if (fgets(dest, size, file) != dest)
        return cant_read_line;
    
    // Remove newline chars from the end
    while (strlen(dest) > 0 && (dest[strlen(dest) - 1] == '\n' || dest[strlen(dest) - 1] == '\r'))
        dest[strlen(dest) - 1] = '\0';

    return exit_success;
}

status_code_t product_read(FILE *file, product_t *product)
{
    assert(file != NULL && product != NULL);

    status_code_t status_code = exit_success;

    if ((status_code = read_line(product->name, MAX_NAME_SIZE, file)) != exit_success)
    {
        status_code = end_of_file;
    }
    else if(((status_code = read_line(product->mfr, MAX_MFR_SIZE, file)) != exit_success
        || fscanf(file, "%" SCNu32 "\n%" SCNu32 "\n", &product->price, &product->amount) != 2))
    {
        status_code = cant_read_product;
    }
    
    return status_code;
}

status_code_t product_write(FILE *file, product_t *product)
{
    assert(file != NULL && product != NULL);

    fprintf(file, "%s\n", product->name);
    fprintf(file, "%s\n", product->mfr);
    fprintf(file, "%" SCNu32 "\n%" SCNu32 "\n", product->price, product->amount);

    return exit_success;
}

bool product_equal(product_t *product_1, product_t *product_2)
{
    assert(product_1 != NULL && product_2 != NULL);

    return strcmp(product_1->name, product_2->name) == 0
        && strcmp(product_1->mfr, product_2->mfr) == 0
        && product_1->price == product_2->price
        && product_1->amount == product_2->amount;
}

bool product_ordered(product_t *product_1, product_t *product_2)
{
    assert(product_1 != NULL && product_2 != NULL);

    if (product_1->price == product_2->price)
        return product_1->amount > product_2->amount;

    return product_1->price > product_2->price;
}

void product_swap(product_t *product_1, product_t *product_2)
{
    assert(product_1 != NULL && product_2 != NULL);

    product_t temp = *product_1;
    *product_1 = *product_2;
    *product_2 = temp;
}
