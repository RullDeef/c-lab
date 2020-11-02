#include <stdio.h>
#include <stdlib.h>
#include "num_dcmp.h"
#include "commander.h"

int main(void)
{
    int status = EXIT_FAILURE;
    char *line = NULL;
    size_t line_len = 0;

    getline(&line, &line_len, stdin);
    if (line != NULL)
    {
        cmd_type_t cmd = cmd_parse(line);
        status = cmd_execute(cmd);
        free(line);
    }

    return status;
}
