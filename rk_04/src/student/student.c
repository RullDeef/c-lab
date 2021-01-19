#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "list/list.h"

struct student
{
    char *name;
    int age;
    list_t marks;
};

student_t stud_create(const char *name, int age)
{
    struct student *stud = malloc(sizeof(struct student));

    if (stud != NULL)
    {
        stud->name = strdup(name);
        stud->age = age;
        stud->marks = lst_create();
    }

    return stud;
}

void stud_destroy(student_t stud)
{
    if (stud != NULL)
    {
        free(stud->name);
        lst_destroy(stud->marks, NULL);
        free(stud);
    }
}

const char *stud_get_name(student_t stud)
{
    return stud == NULL ? "(null)" : stud->name;
}

int stud_append_mark(student_t stud, int mark)
{
    int status = EXIT_FAILURE;

    if (stud != NULL)
    {
        lst_append(stud->marks, (void *)mark);
        status = EXIT_SUCCESS;
    }

    return status;
}

static int __print_marks_list(void *mark)
{
    printf("%d, ", (int)mark);
    return EXIT_SUCCESS;
}

int stud_printf(student_t stud)
{
    int status = EXIT_FAILURE;

    if (stud == NULL)
        printf("(null)\n");
    else
    {
        printf("[%s|%d] (", stud->name, stud->age);
        lst_for_each(stud->marks, __print_marks_list);
        printf(")\n");
        status = EXIT_SUCCESS;
    }

    return status;
}
