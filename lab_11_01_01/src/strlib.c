#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include "strlib.h"

#define MAX_NUM_DIGITS_COUNT 20
#define HEX_BASE 16
#define DEC_BASE 10
#define OCT_BASE 8

static bool fmt_is_valid(const char *format);
static int put_char(char **buf, size_t *n, char c);
static int put_str(char **buf, size_t *n, const char *str);
static int put_number(char **buf, size_t *n, long long int num, int base);

static int parse_format(char **buf, size_t *n, const char **format, va_list *args);

int my_snprintf(char *buf, size_t n, const char *format, ...)
{
    if (!fmt_is_valid(format) || (buf == NULL && n > 0))
        return -1;

    int write = 0;
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%' && *(format++))
            write += parse_format(&buf, &n, &format, &args);
        else
            write += put_char(&buf, &n, *(format++));
    }

    put_char(&buf, &n, '\0');

    va_end(args);
    return write;
}

static bool fmt_is_valid(const char *format)
{
    bool valid = true;

    while (valid && *format)
    {
        if (*format == '%')
            valid = format++ && ((*format == 'c' || *format == 's')
                || ((*format == 'l' || *format == 'h') && format++) || (*format == 'd'
                || *format == 'x' || *format == 'o'));
        format++;
    }

    return valid;
}

static int put_char(char **buf, size_t *n, char c)
{
    if (*n > 0)
        *((*buf)++) = (*n)-- == 1 ? '\0' : c;
    return 1;
}

static int put_str(char **buf, size_t *n, const char *str)
{
    int write = 0;
    for (; *str; str++, write++)
        put_char(buf, n, *str);
    return write;
}


static int put_number(char **buf, size_t *n, long long int num, int base)
{
    char digits[MAX_NUM_DIGITS_COUNT];
    int write = 0;

    bool negative = num < 0;
    if (negative)
    {
        write = put_char(buf, n, '-');
    }

    int i = 0;
    do
    {
        char digit = '0' + (num % base > 0 ? num % base : -(num % base));
        if (digit > '9')
            digit += 'a' - '0' - 10;

        digits[i++] = digit;

        num /= base;
    }
    while (num != 0);

    write += i; // amount of digits

    while (i > 0)
        put_char(buf, n, digits[--i]);

    return write;
}

static int parse_format(char **buf, size_t *n, const char **format, va_list *args)
{
    int write = 0;

    switch (*((*format)++))
    {
        case 'c':
            write += put_char(buf, n, (char)va_arg(*args, int));
            break;
        case 's':
            write += put_str(buf, n, va_arg(*args, const char *));
            break;
        case 'd':
        case 'i':
            write += put_number(buf, n, va_arg(*args, int), DEC_BASE);
            break;
        case 'x':
            write += put_number(buf, n, va_arg(*args, int), HEX_BASE);
            break;
        case 'o':
            write += put_number(buf, n, va_arg(*args, int), OCT_BASE);
            break;
        case 'l':
            switch (*((*format)++))
            {
                case 'd':
                    write += put_number(buf, n, va_arg(*args, long long int), DEC_BASE);
                    break;
                case 'x':
                    write += put_number(buf, n, va_arg(*args, long long int), HEX_BASE);
                    break;
                case 'o':
                    write += put_number(buf, n, va_arg(*args, long long int), OCT_BASE);
                    break;
            }
            break;
        case 'h':
            switch (*((*format)++))
            {
                case 'd':
                    write += put_number(buf, n, va_arg(*args, int), DEC_BASE);
                    break;
                case 'x':
                    write += put_number(buf, n, va_arg(*args, int), HEX_BASE);
                    break;
                case 'o':
                    write += put_number(buf, n, va_arg(*args, int), OCT_BASE);
                    break;
            }
            break;
    }

    return write;
}
