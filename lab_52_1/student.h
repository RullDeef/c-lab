#ifndef __STUDENT_H
#define __STUDENT_H

#include <inttypes.h>
#include <stdbool.h>
#include "defs.h"

#define MAX_SURNAME_LENGTH (25 + 1)
#define MAX_FIRST_NAME_LENGTH (10 + 1)
#define MARKS_AMOUNT 4

typedef uint32_t mark_t;

typedef struct
{
    char surname[MAX_SURNAME_LENGTH];
    char first_name[MAX_FIRST_NAME_LENGTH];
    mark_t marks[MARKS_AMOUNT];
} student_t;

void student_read_binary(FILE *file, size_t pos, student_t *student);
void student_write_binary(FILE *file, size_t pos, const student_t *student);
int student_compare(const void *st_1, const void *st_2);
bool student_startswith(student_t *student, const char *substr);
float student_mean(const student_t *student);

#endif
