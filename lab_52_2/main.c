#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "status_codes.h"
#include "operations.h"

typedef enum
{
    operation_sort,
    operation_find,
    operation_insert
} operation_t;

bool is_args_supported(int argc, char *argv[]);
operation_t get_operation(char op_char);

int main(int argc, char *argv[])
{
    status_code_t status_code = exit_success;

    if (!is_args_supported(argc, argv))
        status_code = unsupported_args;
    else
    {
        switch (get_operation(argv[1][0]))
        {
            case operation_sort:
                status_code = sort_products(argv[2], argv[3]);
                break;
            case operation_find:
                status_code = find_products(argv[2], argv[3]);
                break;
            case operation_insert:
                break;
            default:
                assert(false);
                status_code = exit_failure;
                break;
        }
    }

    return status_code;
}

bool is_args_supported(int argc, char *argv[])
{
    if (argc == 4)
    {
        if (strcmp(argv[1], "st") == 0)
            return true;
        else if (strcmp(argv[1], "ft") == 0)
            return true;
    }
    
    return false;
}

operation_t get_operation(char op_char)
{
    if (op_char == 's')
        return operation_sort;
    else if (op_char == 'f')
        return operation_find;
    else if (op_char == 'a')
        return operation_insert;
    
    assert(false);
    return -1;
}
