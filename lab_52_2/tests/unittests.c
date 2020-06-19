#include <stdio.h>
#include "product.h"
#include "operations.h"

#define TEMP_FILENAME "./tests/.temp"

int testcase_product_read(const char* file_content, status_code_t expected_status, product_t *expected_product);
int test_product_read();

int main(void)
{
    int failed_count = 0;

    failed_count += test_product_read();

    return failed_count;
}

int testcase_product_read(const char* file_content, status_code_t expected_status, product_t *expected_product)
{
    FILE *temp_file;

    // Print out some description about test case
    printf("testcase_product_read: ");

    // Write file content to temp file
    temp_file = fopen(TEMP_FILENAME, "w");
    fprintf(temp_file, "%s", file_content);
    fclose(temp_file);

    temp_file = fopen(TEMP_FILENAME, "r");

    // Run actual function
    product_t actual_product = { .name = "", .mfr = "", .price = 0, .amount = 0 };
    status_code_t actual_status = product_read(temp_file, &actual_product);

    // Check actual with expected
    if (expected_status != actual_status)
    {
        printf("failed - expected status: %d, got: %d\n", expected_status, actual_status);
        return 1;
    }
    else if (actual_status == exit_success && expected_product != NULL)
    {
        if (!product_equal(expected_product, &actual_product))
        {
            printf("failed - expected product:\n");
            product_write(stdout, expected_product);
            printf("  but got:\n");
            product_write(stdout, &actual_product);
            return 1;
        }
    }

    printf("passed\n");
    return 0;
}

int test_product_read()
{
    int failed_count = 0;

    failed_count += testcase_product_read("", end_of_file, NULL);
    failed_count += testcase_product_read("prod\n", cant_read_product, NULL);
    failed_count += testcase_product_read("name\nmanfr\n", cant_read_product, NULL);
    failed_count += testcase_product_read("name\nmanfr\n100\n", cant_read_product, NULL);

    failed_count += testcase_product_read("name\nmanfr\n100\n20\n", exit_success, &(product_t) {
        .name = "name", .mfr = "manfr", .price = 100, .amount = 20
    });

    return failed_count;
}
