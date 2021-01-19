#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "list/list.h"
#include "student/student.h"

#define TEMP_BUFFER_SIZE 10
#define NAME_BUFFER_SIZE 64
#define INVALID_OPT -1

int run_menu(list_t students);

int add_stud(list_t students);
int print_studs(list_t students);
int pop_stud(list_t students);

int main(void)
{
    int status;
    list_t students = lst_create();

    status = run_menu(students);

    if (status == EXIT_FAILURE)
        printf("Меню завершилось аварийно.\n");

    lst_destroy(students, (lst_unary_fn_t)stud_destroy);
    return status;
}

static int __get_menu_opt(void)
{
    int opt;
    char temp[TEMP_BUFFER_SIZE];

    fgets(temp, TEMP_BUFFER_SIZE, stdin);
    if (sscanf(temp, "%d", &opt) != 1)
        opt = INVALID_OPT;

    return opt;
}

int run_menu(list_t students)
{
    int status = EXIT_SUCCESS;
    bool need_exit = false;

    while (!need_exit && status == EXIT_SUCCESS)
    {
        printf("Меню:\n");
        printf("    1. Добавить студента в список\n");
        printf("    2. Вывести список студентов\n");
        printf("    3. Убрать последнего студента в списке\n");
        printf("    0. Выйти\n");

        int opt = __get_menu_opt();
        switch (opt)
        {
            case 0:
                printf("Выход из программы.\n");
                need_exit = true;
                break;

            case 1:
                status = add_stud(students);
                break;
            
            case 2:
                status = print_studs(students);
                break;
            
            case 3:
                status = pop_stud(students);
                break;

            default:
            case INVALID_OPT:
                printf("Неверная опция. Повторите попытку.\n");
                break;
        }
    }

    return status;
}

static int __add_stud_marks(student_t student)
{
    int status = EXIT_SUCCESS;
    printf("Введите оценки студента через пробел в одну строку:\n");
    printf("%s: ", stud_get_name(student));

    char *buffer = NULL;
    size_t size = 0;

    if (getline(&buffer, &size, stdin) < 0)
        printf("Неверный список оценок.\n");
    else
    {
        char *buffer_iter = buffer;
        int mark;

        while (status == EXIT_SUCCESS && sscanf(buffer_iter, "%d", &mark) == 1)
        {
            buffer_iter = strchr(buffer_iter, ' ');
            status = stud_append_mark(student, mark);
            if (buffer_iter == NULL)
                break;
            else
                buffer_iter++;
        }

        if (status == EXIT_SUCCESS)
            printf("Список оценок добавлен.\n");
    }

    free(buffer);
    return status;
}

int add_stud(list_t students)
{
    int status = EXIT_SUCCESS;
    char *buffer = NULL;
    size_t size = 0;

    printf("Введите имя студента и его возраст через пробел: ");

    if (getline(&buffer, &size, stdin) < 0)
        printf("Неверное имя студента!\n");
    else
    {
        char name[NAME_BUFFER_SIZE];
        int age;

        if (sscanf(buffer, "%s%d", name, &age) != 2)
            printf("Неверный ввод!\n");
        else
        {
            student_t student = stud_create(name, age);
            status = lst_append(students, student);
            if (status == EXIT_SUCCESS)
            {
                printf("Добавлен студент: %s\n", name);
                status = __add_stud_marks(student);
            }
        }
    }

    free(buffer);
    return status;
}

static int __stud_printer(void *stud)
{
    stud_printf((student_t)stud);
    return EXIT_SUCCESS;
}

int print_studs(list_t students)
{
    printf("Список студентов:\n");
    lst_for_each(students, __stud_printer);
    return EXIT_SUCCESS;
}

int pop_stud(list_t students)
{
    student_t student;
    int status = lst_pop_back(students, (void **)&student);
    if (status == EXIT_FAILURE)
        printf("Список пуст. Студетов нет.\n");
    else
    {
        printf("Удален студент: ");
        stud_printf(student);
        stud_destroy(student);
    }
    return EXIT_SUCCESS;
}
