#include <stdarg.h>
#include "strlib.h"

static void parse_spec(char *s, size_t *n, size_t *write, const char **format, va_list args);
static void parse_char(char *s, size_t *n, size_t *write, va_list args);
static void parse_percent(char *s, size_t *n, size_t *write);

int my_snprintf(char *s, size_t n, const char *format, ...)
{
    size_t write = 0U;

    va_list args;
    va_start(args, format);

    while (format != NULL && *format != '\0')
    {
        if (*format == '%')
        {
            format++;
            parse_spec(s, &n, &write, &format, args);
        }
        else
        {
            if (n > 0U)
                s[write] = n-- == 1U ? '\0' : *format;

            write++;
            format++;
        }
    }

    va_end(args);

    if (n > 0U)
        s[write] = '\0';

    return write;
}

static void parse_spec(char *s, size_t *n, size_t *write, const char **format, va_list args)
{
    switch (*((*format)++))
    {
        case 'c':
            parse_char(s, n, write, args);
            break;
        case '%':
            parse_percent(s, n, write);
            break;
    }
}

static void parse_char(char *s, size_t *n, size_t *write, va_list args)
{
    char c = (char)va_arg(args, int);

    if (*n > 0U)
        s[*write] = (*n)-- == 1U ? '\0' : c;

    (*write)++;
}

static void parse_percent(char *s, size_t *n, size_t *write)
{
    if (*n > 0U)
        s[*write] = (*n)-- == 1U ? '\0' : '%';

    (*write)++;
}
