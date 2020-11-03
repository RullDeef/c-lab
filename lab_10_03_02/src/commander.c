#include <stdio.h>
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
        struct num_dcmp nd = nd_decompose(number);

        status = nd_output(&nd);

        nd_destroy(&nd);
    }

    return status;
}

static int imp__cmd_sqr(char *line)
{
    int status = EXIT_FAILURE;

    unsigned long number = cmd_parse_number(line);
    if (number != 0UL)
    {
        struct num_dcmp nd = nd_decompose(number);
        struct num_dcmp sq = nd_square(&nd);

        status = nd_output(&sq);

        nd_destroy(&nd);
        nd_destroy(&sq);
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
        struct num_dcmp nd_1 = nd_decompose(number_1);
        struct num_dcmp nd_2 = nd_decompose(number_2);
        struct num_dcmp prod = nd_multiply(&nd_1, &nd_2);

        status = nd_output(&prod);

        nd_destroy(&nd_1);
        nd_destroy(&nd_2);
        nd_destroy(&prod);
    }

    return status;
}

static int imp__cmd_div(char *line_1, char *line_2)
{
    int status = EXIT_FAILURE;

    unsigned long number_1 = cmd_parse_number(line_1);
    unsigned long number_2 = cmd_parse_number(line_2);
    if (number_1 != 0UL && number_2 != 0UL)
    {
        struct num_dcmp nd_1 = nd_decompose(number_1);
        struct num_dcmp nd_2 = nd_decompose(number_2);
        struct num_dcmp res = nd_divide(&nd_1, &nd_2);

        status = nd_output(&res);

        nd_destroy(&nd_1);
        nd_destroy(&nd_2);
        nd_destroy(&res);
    }

    return status;
}

static int imp__unar_cmd_wrapper(int (*cmd)(char *))
{
    char *line = NULL;
    size_t len = 0U;

    int status = EXIT_FAILURE;

    if (getline(&line, &len, stdin) > 0)
        status = cmd(line);

    free(line);
    return status;
}

static int imp__bin_cmd_wrapper(int (*cmd)(char *, char *))
{
    char *line_1 = NULL;
    char *line_2 = NULL;
    size_t line_1_len = 0U;
    size_t line_2_len = 0U;

    int status = EXIT_FAILURE;

    if (getline(&line_1, &line_1_len, stdin) > 0)
    {
        if (getline(&line_2, &line_2_len, stdin) > 0)
            status = cmd(line_1, line_2);
        free(line_2);
    }

    free(line_1);
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
        case cmd_div:
            status = imp__bin_cmd_wrapper(imp__cmd_div);
            break;
        default:
            break;
    }

    return status;
}
