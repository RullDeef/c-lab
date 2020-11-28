#include <stdarg.h>
#include "strlib.h"

#define MAX_DIGITS_COUNT 11

static void parse_spec(char *s, size_t *n, size_t *write, const char **format, va_list *args);
static void parse_decimal(char *s, size_t *n, size_t *write, va_list *args);
static void parse_octal(char *s, size_t *n, size_t *write, va_list *args);
static void parse_hexadecimal(char *s, size_t *n, size_t *write, va_list *args);
static void parse_char(char *s, size_t *n, size_t *write, va_list *args);
static void parse_str(char *s, size_t *n, size_t *write, va_list *args);
static void parse_percent(char *s, size_t *n, size_t *write);

typedef void (*int_parser_t)(int *, char *);
static void dec_parser(int *num, char *digit);
static void oct_parser(int *num, char *digit);
static void hex_parser(int *num, char *digit);

static void parse_int(char *s, size_t *n, size_t *write, int num, int_parser_t parser);

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
            parse_spec(s, &n, &write, &format, &args);
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

static void parse_spec(char *s, size_t *n, size_t *write, const char **format, va_list *args)
{
    switch (*((*format)++))
    {
        case 'x':
            parse_hexadecimal(s, n, write, args);
            break;
        case 'o':
            parse_octal(s, n, write, args);
            break;
        case 'd':
        case 'i':
            parse_decimal(s, n, write, args);
            break;
        case 'c':
            parse_char(s, n, write, args);
            break;
        case 's':
            parse_str(s, n, write, args);
            break;
        case '%':
            parse_percent(s, n, write);
            break;
    }
}

static void parse_decimal(char *s, size_t *n, size_t *write, va_list *args)
{
    int number = va_arg(*args, int);
    parse_int(s, n, write, number, dec_parser);
}

static void parse_octal(char *s, size_t *n, size_t *write, va_list *args)
{
    int number = va_arg(*args, int);
    parse_int(s, n, write, number, oct_parser);
}
static void parse_hexadecimal(char *s, size_t *n, size_t *write, va_list *args)
{
    int number = va_arg(*args, int);
    parse_int(s, n, write, number, hex_parser);
}

static void dec_parser(int *num, char *digit)
{
    *digit = '0' + *num % 10;
    *num /= 10;
}

static void oct_parser(int *num, char *digit)
{
    *digit = '0' + *num % 8;
    *num /= 8;
}

static void hex_parser(int *num, char *digit)
{
    *digit = *num % 16;
    *digit += *digit < 10 ? '0' : 'a' - 10;
    *num /= 16;
}

static void parse_int(char *s, size_t *n, size_t *write, int num, int_parser_t parser)
{
    char digits[MAX_DIGITS_COUNT] = { 0 };

    int sign = num < 0 ? -1 : 1;
    num *= sign;

    int i = MAX_DIGITS_COUNT;
    while (i-- > 0)
        parser(&num, digits + i);

    for (i = 0; i + 1 < MAX_DIGITS_COUNT && digits[i] == '0'; i++);

    if (sign == -1)
    {
        if (*n > 0U)
            s[*write] = (*n)-- == 1U ? '\0' : '-';
        (*write)++;
    }

    while (*n > 0U && i < MAX_DIGITS_COUNT)
    {
        char d = digits[i++];
        s[(*write)++] = (*n)-- == 1U ? '\0' : d;
    }

    if (i < MAX_DIGITS_COUNT)
        *write += MAX_DIGITS_COUNT - i;
}

static void parse_char(char *s, size_t *n, size_t *write, va_list *args)
{
    char c = (char)va_arg(*args, int);

    if (*n > 0U)
        s[*write] = (*n)-- == 1U ? '\0' : c;

    (*write)++;
}

static void parse_str(char *s, size_t *n, size_t *write, va_list *args)
{
    const char *str = va_arg(*args, const char*);

    while (*n > 0U && *str != '\0')
    {
        char c = *(str++);
        s[(*write)++] = (*n)-- == 1U ? '\0' : c;
    }

    while (*(str++) != '\0')
        (*write)++;
}

static void parse_percent(char *s, size_t *n, size_t *write)
{
    if (*n > 0U)
        s[*write] = (*n)-- == 1U ? '\0' : '%';

    (*write)++;
}
