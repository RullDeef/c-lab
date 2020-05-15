#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "student.h"

/*
void remove_newline(char *str)
{
    size_t len = strlen(str);
    if (str[len - 1] == '\n')
        str[len - 1] = '\0';
}

status_code_t student_read_text(FILE *file, student_t *student)
{
    char marks_buffer[MAX_MARKS_LINE_LENGTH];
    status_code_t status_code = exit_success;

    if (fgets(student->surname, MAX_SURNAME_LENGTH, file) != student->surname)
        status_code = end_of_file;
    else if (fgets(student->first_name, MAX_FIRST_NAME_LENGTH, file) != student->first_name)
        status_code = invalid_file_format;
    else if (strlen(student->surname) > MAX_SURNAME_LENGTH)
        status_code = too_long_surname;
    else if (strlen(student->first_name) > MAX_FIRST_NAME_LENGTH)
        status_code = too_long_first_name;
    else
    {
        remove_newline(student->surname);
        remove_newline(student->first_name);

        for (size_t i = 0; i < MARKS_AMOUNT; i++)
        {
            int res = fscanf(file, "%ud", &(student->marks[i]));
            if (res == EOF || res != 1)
            {
                status_code = invalid_file_format;
                break;
            }
        }

        fgets(marks_buffer, MAX_MARKS_LINE_LENGTH, file);
    }

    return status_code;
}

status_code_t student_write_text(FILE *file, const student_t *student)
{
    status_code_t status_code = exit_success;

    fprintf(file, "%s\n%s\n", student->surname, student->first_name);

    for (size_t i = 0; i < MARKS_AMOUNT; i++)
        fprintf(file, " %ud" + (i == 0), student->marks[i]);
    fprintf(file, "\n");
    
    return status_code;
}
*/

void student_read_binary(FILE *file, size_t pos, student_t *student)
{
    fseek(file, (long)pos * sizeof(student_t), SEEK_SET);
    fread(student, sizeof(student_t), 1, file);
}

void student_write_binary(FILE *file, size_t pos, const student_t *student)
{
    fseek(file, (long)pos * sizeof(student_t), SEEK_SET);
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
