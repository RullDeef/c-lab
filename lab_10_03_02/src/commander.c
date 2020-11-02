#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "commander.h"
#include "num_dcmp.h"

cmd_type_t cmd_parse(char *str)
{
    cmd_type_t res = cmd_invalid;

    while (isspace((int)*str))
        str++;

    int len = strlen(str);
    while (len > 3 && isspace((int)str[len - 1]))
        str[--len] = '\0';

    if (strcmp(str, "out") == 0)
        res = cmd_out;
    else if (strcmp(str, "mul") == 0)
        res = cmd_mul;
    else if (strcmp(str, "sqr") == 0)
        res = cmd_sqr;
    else if (strcmp(str, "div") == 0)
        res = cmd_div;

    return res;
}

unsigned long cmd_parse_number(const char *str)
{
    char *end = NULL;
    long long res = strtoll(str, &end, 10);

    if (res <= 0LL)
        res = 0LL;

    return res;
}

static int imp__cmd_out(char *line)
{
    int status = EXIT_FAILURE;

    unsigned long number = cmd_parse_number(line);
    if (number != 0UL)
    {
        struct num_dcmp nd = ndcmp_decompose(number);

        ndcmp_output(&nd);

        ndcmp_destroy(&nd);
        status = EXIT_SUCCESS;
    }

    return status;
}

static int imp__cmd_sqr(char *line)
{
    int status = EXIT_FAILURE;

    unsigned long number = cmd_parse_number(line);
    if (number != 0UL)
    {
        struct num_dcmp nd = ndcmp_decompose(number);
        struct num_dcmp sq = ndcmp_square(&nd);

        ndcmp_output(&sq);

        ndcmp_destroy(&nd);
        ndcmp_destroy(&sq);
        status = EXIT_SUCCESS;
    }

    return status;
}

static int imp__cmd_mul(char *line_1, char *line_2)
{
    int status = EXIT_FAILURE;

    unsigned long number_1 = cmd_parse_number(line_1);
    unsigned long number_2 = cmd_parse_number(line_2);
    if (number_1 != 0UL && number_2 != 0UL)
    {
        struct num_dcmp nd_1 = ndcmp_decompose(number_1);
        struct num_dcmp nd_2 = ndcmp_decompose(number_2);
        struct num_dcmp prod = ndcmp_multiply(&nd_1, &nd_2);

        ndcmp_output(&prod);

        ndcmp_destroy(&nd_1);
        ndcmp_destroy(&nd_2);
        ndcmp_destroy(&prod);
        status = EXIT_SUCCESS;
    }

    return status;
}

static int imp__unar_cmd_wrapper(int (*cmd)(char *))
{
    char *line = NULL;
    size_t len = 0U;

    int status = EXIT_FAILURE;

    getline(&line, &len, stdin);
    if (line != NULL)
    {
        status = cmd(line);
        free(line);
    }

    return status;
}

static int imp__bin_cmd_wrapper(int (*cmd)(char *, char *))
{
    char *line_1 = NULL;
    char *line_2 = NULL;
    size_t line_1_len = 0U;
    size_t line_2_len = 0U;

    int status = EXIT_FAILURE;

    getline(&line_1, &line_1_len, stdin);
    if (line_1 != NULL)
    {
        getline(&line_2, &line_2_len, stdin);
        if (line_2 != NULL)
        {
            status = cmd(line_1, line_2);
            free(line_2);
        }
        free(line_1);
    }

    return status;
}

int cmd_execute(cmd_type_t cmd)
{
    int status = EXIT_FAILURE;

    switch (cmd)
    {
    case cmd_out:
        status = imp__unar_cmd_wrapper(imp__cmd_out);
        break;
    
    case cmd_sqr:
        status = imp__unar_cmd_wrapper(imp__cmd_sqr);
        break;
    
    case cmd_mul:
        status = imp__bin_cmd_wrapper(imp__cmd_mul);
        break;

    default:
        break;
    }

    return status;
}
