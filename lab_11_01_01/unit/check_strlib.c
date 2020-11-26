#include <stdio.h>
#include "strlib.h"

int main(void)
{
    char buf[100];

    int write = my_snprintf(buf, 8, "he%clo!%% wait", 'l');

    printf("buf is: \"%s\"\n", buf);
    printf("write %d bytes\n", write);

    write = my_snprintf(buf, 100, "");

    printf("buf is: \"%s\"\n", buf);
    printf("write %d bytes\n", write);

    write = my_snprintf(NULL, 0, "what %c?!", 'A');

    printf("buf is: \"%s\"\n", buf);
    printf("write %d bytes\n", write);

    return 0;
}
