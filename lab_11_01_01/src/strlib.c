#include <stdarg.h>
#include <stdbool.h>
#include "strlib.h"

#define EXIT_FAILURE -1

#define FORMAT_SPEC_SYM '%'

#define MAX_DIGITS_COUNT 21
#define HEX_BASE 16
#define DEC_BASE 10
#define OCT_BASE 8

#define HEX_DIGIT_SHIFT ('a' - '0' - 10)

static bool fmt_is_valid(const char *format);
static void put_char(char **buf, size_t *n, char c);
static int put_str(char **buf, size_t *n, const char *str);
static int put_number(char **buf, size_t *n, long long num, int base);

static int parse_format(char **buf, size_t *n, const char **format, va_list *args);

int my_snprintf(char *buf, size_t n, const char *format, ...)
{
    if (!fmt_is_valid(format) || (buf == NULL && n > 0))
        return EXIT_FAILURE;

    int write = 0;
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == FORMAT_SPEC_SYM && *(format++))
            write += parse_format(&buf, &n, &format, &args);
        else
        {
            put_char(&buf, &n, *(format++));
            write++;
        }
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
        if (*format == FORMAT_SPEC_SYM)
            valid = format++ && ((*format == 'c' || *format == 's' || *format == 'i')
                || ((*format == 'l' || *format == 'h') && format++) || (*format == 'd'
                || *format == 'x' || *format == 'o'));
        format++;
    }

    return valid;
}

static void put_char(char **buf, size_t *n, char c)
{
    if (*n > 0)
        *((*buf)++) = (*n)-- == 1 ? '\0' : c;
}

static int put_str(char **buf, size_t *n, const char *str)
{
    int write = 0;

    for (; *str; write++)
        put_char(buf, n, *(str++));

    return write;
}

static char get_last_digit(long long num, int base)
{
    char digit;

    if (num >= 0)
    {
        if (num >= base)
            digit = (num - base) % base;
        else
            digit = num;
    }
    else
        digit = -(num % base);

    digit += '0';
    if (digit > '9')
        digit += HEX_DIGIT_SHIFT;

    return digit;
}

static int extract_digits(char *digits, long long num, int base)
{
    int digits_count = 0;

    do
    {
        digits[digits_count++] = get_last_digit(num, base);
        num /= base;
    }
    while (num != 0);

    return digits_count;
}

static int put_number(char **buf, size_t *n, long long num, int base)
{
    char digits[MAX_DIGITS_COUNT];
    int digits_count;
    int write = 0;

    if (num < 0 && base == DEC_BASE)
    {
        put_char(buf, n, '-');
        write = 1;
    }

    digits_count = extract_digits(digits, num, base);
    write += digits_count;

    while (digits_count > 0)
        put_char(buf, n, digits[--digits_count]);

    return write;
}

static int parse_format(char **buf, size_t *n, const char **format, va_list *args)
{
    int write = 0;

    switch (*((*format)++))
    {
        case 'c':
            put_char(buf, n, (char)va_arg(*args, int));
            write++;
            break;
        case 's':
            write += put_str(buf, n, va_arg(*args, const char *));
            break;
        case 'd':
        case 'i':
            write += put_number(buf, n, va_arg(*args, int), DEC_BASE);
            break;
        case 'x':
            write += put_number(buf, n, va_arg(*args, unsigned int), HEX_BASE);
            break;
        case 'o':
            write += put_number(buf, n, va_arg(*args, unsigned int), OCT_BASE);
            break;
        case 'l':
            switch (*((*format)++))
            {
                case 'd':
                    write += put_number(buf, n, va_arg(*args, long), DEC_BASE);
                    break;
                case 'x':
                    write += put_number(buf, n, va_arg(*args, unsigned long), HEX_BASE);
                    break;
                case 'o':
                    write += put_number(buf, n, va_arg(*args, unsigned long), OCT_BASE);
                    break;
            }
            break;
        case 'h':
            switch (*((*format)++))
            {
                case 'd':
                    write += put_number(buf, n, (short)va_arg(*args, int), DEC_BASE);
                    break;
                case 'x':
                    write += put_number(buf, n, (unsigned short)va_arg(*args, unsigned int), HEX_BASE);
                    break;
                case 'o':
                    write += put_number(buf, n, (unsigned short)va_arg(*args, unsigned int), OCT_BASE);
                    break;
            }
            break;
    }

    return write;
}
