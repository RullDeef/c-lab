#ifndef __STUDENT_H_
#define __STUDENT_H_

#include <stdio.h>

typedef struct student *student_t;

student_t stud_create(const char *name, int age);
void stud_destroy(student_t stud);

const char *stud_get_name(student_t stud);

int stud_append_mark(student_t stud, int mark);

int stud_printf(student_t stud);

#endif // __STUDENT_H_
