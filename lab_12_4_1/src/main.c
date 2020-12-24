#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "array.h"

#ifdef ARRAY_PLUGIN

#ifdef WIN32
#include <windows.h>
#define LIB_HANDLE HMODULE
#define LIB_LOAD(libname) LoadLibrary(libname)
#define LIB_SYM(handle, symbol) GetProcAddress(handle, symbol)
#define LIB_FREE(handle) FreeLibrary(handle)
#else
#include <dlfcn.h>
#define LIB_HANDLE void *
#define LIB_LOAD(libname) dlopen(libname, RTLD_NOW)
#define LIB_SYM(handle, symbol) dlsym(handle, symbol)
#define LIB_FREE(handle) dlclose(handle)
#endif
typedef struct array(ARR_CALL *arr_create_fn_t)(size_t);
typedef void(ARR_CALL *arr_destroy_fn_t)(struct array *);
typedef int(ARR_CALL *arr_fscanf_fn_t)(FILE *, struct array *);
typedef int(ARR_CALL *arr_fprintf_fn_t)(FILE *, struct array);
typedef void(ARR_CALL *arr_sort_fn_t)(struct array);
typedef int(ARR_CALL *arr_filter_fn_t)(struct array, struct array *);
#endif

#define FILTRATION_FLAG "f"

int process(FILE *ifstream, FILE *ofstream, bool filtration);

int main(int argc, const char **argv)
{
    int status = EXIT_FAILURE;

    if (argc == 3 || argc == 4)
    {
        bool filtration = argc == 4 && strcmp(argv[3], FILTRATION_FLAG) == 0;

        FILE *ifstream = fopen(argv[1], "rt");
        if (ifstream != NULL)
        {
            FILE *ofstream = fopen(argv[2], "wt");
            if (ofstream != NULL)
            {
                status = process(ifstream, ofstream, filtration);
                fclose(ofstream);
            }

            fclose(ifstream);
        }
    }

    return status;
}

int process(FILE *ifstream, FILE *ofstream, bool filtration)
{
    int status = EXIT_FAILURE;

#ifdef ARRAY_PLUGIN
    LIB_HANDLE lib_arr_handle = LIB_LOAD(ARR_LIB_PATH);
    if (lib_arr_handle == NULL)
    {
        fprintf(stderr, "error: could not load plugin " ARR_LIB_PATH "\n");
        status = EXIT_FAILURE;
    }
    else
    {
        // load library functions
        arr_create_fn_t arr_create = (arr_create_fn_t)LIB_SYM(lib_arr_handle, "arr_create");
        arr_destroy_fn_t arr_destroy = (arr_destroy_fn_t)LIB_SYM(lib_arr_handle, "arr_destroy");
        arr_fscanf_fn_t arr_fscanf = (arr_fscanf_fn_t)LIB_SYM(lib_arr_handle, "arr_fscanf");
        arr_fprintf_fn_t arr_fprintf = (arr_fprintf_fn_t)LIB_SYM(lib_arr_handle, "arr_fprintf");
        arr_sort_fn_t arr_sort = (arr_sort_fn_t)LIB_SYM(lib_arr_handle, "arr_sort");
        arr_filter_fn_t arr_filter = (arr_filter_fn_t)LIB_SYM(lib_arr_handle, "arr_filter");
#endif

        struct array arr = arr_create(ARRAY_INITIAL_CAPACITY);
        arr_fscanf(ifstream, &arr);

        if (filtration)
        {
            struct array filtered;
            arr_filter(arr, &filtered);
            arr_destroy(&arr);
            arr = filtered;
        }

        arr_sort(arr);
        arr_fprintf(ofstream, arr);

        arr_destroy(&arr);

#ifdef ARRAY_PLUGIN
        LIB_FREE(lib_arr_handle);
    }
#endif

    return status;
}
