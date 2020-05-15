#ifndef __STUDENT_H
#define __STUDENT_H

#include <inttypes.h>
#include <stdbool.h>
#include "defs.h"

#define MAX_SURNAME_LENGTH (25 + 3)
#define MAX_FIRST_NAME_LENGTH (10 + 2)
#define MAX_MARKS_LINE_LENGTH 255

typedef uint32_t mark_t;

#define MARKS_AMOUNT 4
#define MIN_MARK 2
#define MAX_MARK 5

typedef struct
{
    char surname[MAX_SURNAME_LENGTH];
    char first_name[MAX_FIRST_NAME_LENGTH];
    mark_t marks[MARKS_AMOUNT];
} student_t;

// status_code_t student_read_text(FILE *file, student_t *student);
// status_code_t student_write_text(FILE *file, const student_t *student);
void student_read_binary(FILE *file, size_t pos, student_t *student);
void student_write_binary(FILE *file, size_t pos, const student_t *student);
int student_compare(const void *st_1, const void *st_2);
bool student_startswith(student_t *student, const char *substr);
float student_mean(const student_t *student);

#endif
