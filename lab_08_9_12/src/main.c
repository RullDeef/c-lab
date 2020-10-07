#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "io/app_params.h"
#include "io/mat_io.h"

status_code_t input_matrix(filename_t filename, matrix_t *matrix, input_fn_t input_ft)
{
    FILE *file = fopen(filename, "rt");
    status_code_t status_code = success;

    if (!file)
        status_code = invalid_filename;
    else
    {
        if (input_ft(file, matrix)) // if input failed
            status_code = invalid_file;

        fclose(file);
    }

    return status_code;
}

status_code_t output_matrix(filename_t filename, const matrix_t *matrix, output_fn_t output_fn)
{
    FILE *file = fopen(filename, "wt");
    status_code_t status_code = success;

    if (!file)
        status_code = invalid_filename;
    else
    {
        if (output_fn(file, matrix)) // if output failed
            status_code = invalid_file;

        fclose(file);
    }

    return status_code;
}

status_code_t do_mat_bin_op(filename_t ifname_1, filename_t ifname_2, filename_t ofname,
                            int (*bin_op_fn)(const matrix_t *, const matrix_t *, matrix_t *),
                            input_fn_t input_fn, output_fn_t output_fn)
{
    status_code_t status_code = success;

    matrix_t mat_1 = mat_null();
    matrix_t mat_2 = mat_null();

    if ((status_code = input_matrix(ifname_1, &mat_1, input_fn)) == success &&
        (status_code = input_matrix(ifname_2, &mat_2, input_fn)) == success)
    {
        matrix_t res_mat = mat_create(mat_1.rows, mat_1.cols);

        if (bin_op_fn(&mat_1, &mat_2, &res_mat))
            status_code = invalid_mat_dims;
        else // addition was successful
            status_code = output_matrix(ofname, &res_mat, output_fn);

        mat_free(&res_mat);
    }

    mat_free(&mat_1);
    mat_free(&mat_2);

    return status_code;
}

status_code_t do_ssle(filename_t ifname, filename_t ofname, input_fn_t input_fn, output_fn_t output_fn)
{
    status_code_t status_code = success;

    matrix_t mat = mat_null();

    if ((status_code = input_matrix(ifname, &mat, input_fn)) == success)
    {
        // solve system of linear equations here
        assert(NULL); // not implemented
    }

    mat_free(&mat);

    return status_code;
}

status_code_t do_param_work(app_params_t *app_params, input_fn_t input_fn, output_fn_t output_fn)
{
    status_code_t status_code = success;

    switch (app_params->command)
    {
    case command_add:
        status_code = do_mat_bin_op(app_params->ifname_1, app_params->ifname_2, app_params->ofname, mat_add, input_fn, output_fn);
        break;

    case command_mult:
        status_code = do_mat_bin_op(app_params->ifname_1, app_params->ifname_2, app_params->ofname, mat_add, input_fn, output_fn);
        break;

    case command_ssle:
        status_code = do_ssle(app_params->ifname_1, app_params->ofname, input_fn, output_fn);
        break;

    default:
        status_code = invalid_command;
        break;
    }

    return status_code;
}

status_code_t do_main_work(int argc, const char **argv, input_fn_t input_fn, output_fn_t output_fn)
{
    app_params_t *app_params = parse_app_params(argc, argv);
    status_code_t status_code = success;

    if (!app_params)
        status_code = invalid_app_params;
    else
        status_code = do_param_work(app_params, input_fn, output_fn);

    free(app_params);
    return status_code;
}

int main(int argc, const char **argv)
{
    input_fn_t input_fn = mat_io_input_simple;
    output_fn_t output_fn = mat_io_output_coordinate;

    return do_main_work(argc, argv, input_fn, output_fn);
}