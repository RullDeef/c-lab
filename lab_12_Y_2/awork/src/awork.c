#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

bool is_full_square(long number)
{
    bool result = false;

    for (long i = 1; i * i <= number; i++)
    {
        if (number % i == 0 && number / i == i)
        {
            result = true;
            break;
        }
    }

    return result;
}

static PyObject *method_fill_prime(PyObject *PY_UNUSED(self), PyObject *args)
{
    int size;
    long *primes;

    if (!PyArg_ParseTuple(args, "i", &size))
    {
        PyErr_SetString(PyExc_TypeError, "fucntion takes single argument - size of new array");
        return NULL;
    }

    if (size <= 0)
    {
        PyErr_Format(PyExc_ValueError, "size must be positive. Got %d", size);
        return NULL;
    }

    primes = malloc(size * sizeof(long));

    if (primes == NULL)
    {
        PyErr_Format(PyExc_MemoryError, "unable to allocate %d bytes", size * sizeof(long));
        return NULL;
    }

    PyObject *array = PyList_New(size);

    long num = 1L;

    for (int i = 0; i < size; i++)
    {
        // find next prime number
        bool num_is_prime = false;

        while (!num_is_prime)
        {
            num++;
            num_is_prime = true;

            for (int j = 0; j < i; j++)
            {
                if (num % primes[j] == 0)
                {
                    num_is_prime = false;
                    break;
                }
            }
        }

        primes[i] = num;
        PyList_SetItem(array, i, PyLong_FromLong(num));
    }

    free(primes);
    return array;
}

static PyObject *method_fill_fib(PyObject *Py_UNUSED(self), PyObject *args)
{
    int size;

    if (!PyArg_ParseTuple(args, "i", &size))
    {
        PyErr_SetString(PyExc_TypeError, "function takes single argument - size of new array");
        return NULL;
    }

    if (size <= 0)
    {
        PyErr_Format(PyExc_ValueError, "size must be positive. Got %d", size);
        return NULL;
    }

    PyObject *array = PyList_New(size);

    long curr = 1L;
    long next = 1L;

    for (int i = 0; i < size; i++)
    {
        PyList_SetItem(array, i, PyLong_FromLong(curr));
        long new_next = curr + next;
        curr = next;
        next = new_next;
    }

    return array;
}

static PyObject *method_cycle_shift(PyObject *PY_UNSUSED(self), PyObject *args)
{
    PyObject *array;
    int k;

    if (!PyArg_ParseTuple(args, "Oi", &array, &k))
    {
        PyErr_SetString(PyExc_TypeError, "function takes two arguments - array and integer k");
        return NULL;
    }

    if (!PyList_Check(array))
    {
        PyErr_SetString(PyExc_TypeError, "first argument is not an array (list)");
        return NULL;
    }

    int size = (int)PyList_GET_SIZE(array);
    k = (k % size + size) % size;

    PyObject *left = PyList_GetSlice(array, 0, k);
    PyObject *right = PyList_GetSlice(array, k, size);

    PyList_SetSlice(array, k, size, left);
    PyList_SetSlice(array, 0, k, right);

    Py_DECREF(left);
    Py_DECREF(right);

    return Py_None;
}

static PyObject *method_pull_squares(PyObject *PY_UNSUSED(self), PyObject *args)
{
    PyObject *array_from, *array_to;

    if (!PyArg_ParseTuple(args, "OO", &array_from, &array_to))
    {
        PyErr_SetString(PyExc_TypeError, "function takes two arguments - list <from> and list <to>");
        return NULL;
    }

    if (!PyList_Check(array_from))
    {
        PyErr_SetString(PyExc_TypeError, "first argument is not an array (list)");
        return NULL;
    }

    if (!PyList_Check(array_to))
    {
        PyErr_SetString(PyExc_TypeError, "second argument is not an array (list)");
        return NULL;
    }

    int size = (int)PyList_GET_SIZE(array_from);
    for (int i = 0; i < size; i++)
    {
        PyObject *item = PyList_GET_ITEM(array_from, i);
        if (PyLong_Check(item))
        {
            long number = PyLong_AsLong(item);
            if (is_full_square(number))
            {
                PyList_Append(array_to, item);
                PyList_SetSlice(array_from, i, i + 1, NULL);
                i--;
                size--;
            }
        }
    }

    return Py_None;
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
