#include <stdio.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

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

static PyMethodDef AworkMethods[] = {
    { "fill_fib", method_fill_fib, METH_VARARGS, "Fills array with first n fib nums" },
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
