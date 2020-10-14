#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "io/app_params.h"
#include "io/mat_io.h"

typedef int (*bin_op_fn_t)(const matrix_t *, const matrix_t *, matrix_t *);

/**
 * @brief Считывает матрицу из файла передоваемой функцией чтения.
 */
status_code_t input_matrix(filename_t filename, matrix_t *matrix, input_fn_t input_ft)
{
    FILE *file = fopen(filename, "rt");
    status_code_t status_code = success;

    if (!file)
        status_code = invalid_filename;
    else
    {
        if (input_ft(file, matrix))
            status_code = invalid_file;

        fclose(file);
    }

    return status_code;
}

/**
 * @brief Выводит матрицу в файл с помощью передаваемой функции записи.
 * 
 * @param pricision - кол-во цифр после точки.
 */
status_code_t output_matrix(filename_t filename, const matrix_t *matrix, output_fn_t output_fn, int precision)
{
    FILE *file = fopen(filename, "wt");
    status_code_t status_code = success;

    if (!file)
        status_code = invalid_filename;
    else
    {
        if (output_fn(file, matrix, precision))
            status_code = invalid_file;

        fclose(file);
    }

    return status_code;
}

/**
 * @brief Выполняет операцию над матрицами, хранящимися в файлах
 * с указанными именами и выводит результат в выходной файл.
 */
status_code_t do_mat_bin_op(filename_t ifname_1, filename_t ifname_2, filename_t ofname, bin_op_fn_t bin_op_fn, input_fn_t input_fn, output_fn_t output_fn)
{
    status_code_t status_code = success;

    matrix_t mat_1 = mat_null();
    matrix_t mat_2 = mat_null();

    if ((status_code = input_matrix(ifname_1, &mat_1, input_fn)) == success &&
        (status_code = input_matrix(ifname_2, &mat_2, input_fn)) == success)
    {
        matrix_t res_mat = mat_null();

        if (bin_op_fn(&mat_1, &mat_2, &res_mat) != mat_success)
            status_code = invalid_mat_dims;
        else
            status_code = output_matrix(ofname, &res_mat, output_fn, MAT_IO_DOUBLE_PRECISION);

        mat_free(&res_mat);
    }

    mat_free(&mat_1);
    mat_free(&mat_2);

    return status_code;
}

/**
 * @brief Решает СЛАУ, представимую в виде матрицы во
 * входном файле и выводит результат в выходной файл.
 */
status_code_t do_ssle(filename_t ifname, filename_t ofname, input_fn_t input_fn, output_fn_t output_fn)
{
    status_code_t status_code = success;

    matrix_t mat = mat_null();
    matrix_t res = mat_null();

    if ((status_code = input_matrix(ifname, &mat, input_fn)) == success)
    {
        if (mat_solve_sle(&mat, &res) != mat_success)
            status_code = failure;
        else
            status_code = output_matrix(ofname, &res, output_fn, MAT_IO_DOUBLE_PRECISION);
    }

    mat_free(&mat);
    mat_free(&res);

    return status_code;
}

/**
 * @brief Решает одну задачу, в зависимости от передаваемых параметров.
 */
status_code_t do_param_work(app_params_t *app_params, input_fn_t input_fn, output_fn_t output_fn)
{
    status_code_t status_code = success;

    switch (app_params->command)
    {
        case command_add:
            status_code = do_mat_bin_op(app_params->ifname_1, app_params->ifname_2, app_params->ofname, mat_add, input_fn, output_fn);
            break;
        case command_mult:
            status_code = do_mat_bin_op(app_params->ifname_1, app_params->ifname_2, app_params->ofname, mat_mult, input_fn, output_fn);
            break;
        case command_ssle:
            status_code = do_ssle(app_params->ifname_1, app_params->ofname, input_fn, output_fn);
            break;
    }

    return status_code;
}

/**
 * @brief Организует всю работу программы, принимая специальные функции ввода и вывода.
 */
status_code_t do_main_work(int argc, const char **argv, input_fn_t input_fn, output_fn_t output_fn)
{
    app_params_t *app_params = parse_app_params(argc, argv);
    status_code_t status_code = success;

    if (!app_params)
        status_code = invalid_app_params;
    else
    {
        status_code = do_param_work(app_params, input_fn, output_fn);
        free(app_params);
    }

    return status_code;
}

int main(int argc, const char **argv)
{
    input_fn_t input_fn = mat_io_input_simple;
    output_fn_t output_fn = mat_io_output_coordinate;

    return do_main_work(argc, argv, input_fn, output_fn);
}
