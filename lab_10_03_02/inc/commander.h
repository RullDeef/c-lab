#ifndef __COMMANDER_H_
#define __COMMANDER_H_

#include <stdio.h>

typedef char cmd_type_t;

enum
{
    cmd_invalid = 0,
    cmd_out,
    cmd_mul,
    cmd_sqr,
    cmd_div
};

cmd_type_t cmd_parse(char *str);
unsigned long cmd_parse_number(const char *str);

int cmd_execute(cmd_type_t cmd);

#endif // __COMMANDER_H_
