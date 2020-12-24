# Отчёт по лабораторной работе 12

---

## Задание

Реализовать программу на языке Python, в которой используются функции, написанные на С. Написать модуль библиотеки и импортировать его в программу.

---

## Алгоритм создания модуля расширения

Для создания модуля расширения необходимо определённым образом оформить файл с С-функциями, экспортируемыми в модуль, и собрать модуль с помощью самого питона.

Условное оформление модуля расширения:

```C
// foo.c

// подключение заголовочного файла с определениями объектов питона
#include <Python.h>

// написание непосредственно экспортируемой функции.
// Внимание. Функция обязательно должна быть статической
static PyObject *method_foo(PyObject *self, PyObject *args)
{
    // ...
}

// здесь должны быть указаны все экспортируемые функции,
// их соответственные имена в питоне и строка документации
// для описания работы функций
static PyMethodDef FooMethods[] = {
    { "foo", method_foo, METH_VARARGS, "this is simple C function" },
    // массив должен оканчиваться нулевой структурой
    { NULL, NULL, 0, NULL }
};

// далее идет описание самого модуля
static struct PyModuleDef FooModule = {
    PyModuleDef_HEAD_INIT, // корректный отступ для специфичных полей
    "foo module", // название модуля
    "simple module written in C", // строка документации
    -1,
    FooMethods // массив экспортируемых методов
};

// функция создания модуля (вызывается при импорте)
PyMODINIT_FUNC PyInit_foo_module(void)
{
    return PyModule_Create(&FooModule);
}
```

Далее необходимо написать скрипт для сборки модуля расширения. В 99% случаев он выглядит так:

```Python
# setup.py

from distutils.core import setup, Extension

setup(
    name="foo module",
    version="1.0.0",
    description="simple module written in C",
    author="author name",
    author_email="author.mail@example.com",
    ext_modules=[Extension("foomodule", ["./foo.c"])]
)
```

Для сборки остаётся лишь вызвать написанный скрипт следующим образом:

```Shell
python setup.py build
```

После успешной сборки в текущей папке появится дериктория `build` в которой и располагается скомпилированный и готовый к использованию модуль. При желании его можно переименовать и переместить в папку с основным проектом.

---

## Исходные файлы

### Файл `awork.c`

```C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *method_fill_prime(PyObject *PY_UNUSED(self), PyObject *args)
{
    // ...
}

static PyObject *method_fill_fib(PyObject *Py_UNUSED(self), PyObject *args)
{
    // ...
}

static PyObject *method_cycle_shift(PyObject *PY_UNSUSED(self), PyObject *args)
{
    // ...
}

static PyObject *method_pull_squares(PyObject *PY_UNSUSED(self), PyObject *args)
{
    // ...
}

static PyMethodDef AworkMethods[] = {
    { "fill_fib", method_fill_fib, METH_VARARGS, "Fills array with first n fib nums" },
    { "fill_prime", method_fill_prime, METH_VARARGS, "Fills array with first n prime nums" },
    { "cycle_shift", method_cycle_shift, METH_VARARGS, "Shifts array by k positions" },
    { "pull_squares", method_pull_squares, METH_VARARGS, "Pulls squares from one array to another" },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef AworkModule = {
    PyModuleDef_HEAD_INIT,
    "awork",
    "tiny module for working with arrays",
    -1,
    AworkMethods
};

PyMODINIT_FUNC PyInit_awork(void)
{
    return PyModule_Create(&AworkModule);
}
```

### Файл `setup.py`

```Python
from distutils.core import setup, Extension

setup(
    name="awork",
    version="1.0.0",
    description="tiny module implemented in C",
    author="Klimenko Alexey",
    author_email="klimenko0037@gmail.com",
    ext_modules=[Extension("awork", ["./src/awork.c"])]
)
```

### Файл `app.py`

```Python
# ...

# импорт С-функций из модуля расширения
from awork import fill_fib, fill_prime, cycle_shift, pull_squares

# ...
```
