#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "student.h"

void student_read_binary(FILE *file, size_t pos, student_t *student)
{
    fseek(file, pos * sizeof(student_t), SEEK_SET);
    fread(student, sizeof(student_t), 1, file);
}

void student_write_binary(FILE *file, size_t pos, const student_t *student)
{
    fseek(file, pos * sizeof(student_t), SEEK_SET);
    fwrite(student, sizeof(student_t), 1, file);
}

int student_compare(const void *st_1, const void *st_2)
{
    const student_t *student_1 = (const student_t *)st_1;
    const student_t *student_2 = (const student_t *)st_2;
    if (strcmp(student_1->surname, student_2->surname) == 0)
        return strcmp(student_1->first_name, student_2->first_name);
    return strcmp(student_1->surname, student_2->surname);
}

bool student_startswith(student_t *student, const char *substr)
{
    return strstr(student->surname, substr) != NULL;
}

float student_mean(const student_t *student)
{
    float mean = 0.0f;

    for (size_t i = 0; i < MARKS_AMOUNT; i++)
        mean += (float)student->marks[i];
    
    mean /= (float)MARKS_AMOUNT;
    return mean;
}
