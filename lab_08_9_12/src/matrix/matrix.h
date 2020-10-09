/**
 * @file matrix.h
 * @breif Модуль для обработки динамических матриц.
 */

#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <stdbool.h>

/**
 * @brief статус-коды операций реализованных в данном модуле.
 */
enum
{
    mat_success = 0,
    mat_bad_create,
    mat_bad_dims,
    mat_bad_copy,
    mat_bad_alloc,
    mat_bad_resize,
    mat_bad_matrix,
    mat_zero_matrix,
    mat_singular_matrix,
};

typedef double matrix_elem_t;

/**
 * @brief Структура динамической матрицы.
 * 
 * Для хранения элементов матрицы используется объединенный подход 2:
 * вся матрица и указатели на строки располагаются одним блоком.
 * 
 * @param rows - количество строк матрицы
 * @param cols - количество столбцов матрицы
 * @param imp__allocated - размер выделенной памяти (для обработки внутри модуля)
 * @param data - динамический блок данных состоящий из указателей
 *               на строки и самих строк матрицы.
 */
typedef struct
{
    size_t rows;
    size_t cols;
    size_t imp__allocated;
    matrix_elem_t **data;
} matrix_t;

/**
 * @brief Возвращает NULL-матрицу.
 * 
 * Для создания NULL-матрицы не выделяется динамеческая память.
 * Их можно использовать для обнуления неинициализированных матриц.
 */
matrix_t mat_null(void);

/**
 * @brief Создаёт нулевую матрицу заданного размера.
 * 
 * При нулевых размерах возвращается NULL-матрица.
 */
matrix_t mat_create(size_t rows, size_t cols);

/**
 * @brief Проверяет на равенство NULL-матрице.
 */
bool mat_is_null(const matrix_t *matrix);

/**
 * @brief Очищает память и делает матрицу NULL-матрицей.
 * 
 * Если матрица уже оцищена, ничего не делает.
 */
void mat_free(matrix_t *matrix);

/**
 * @brief Копирует матрицу и возвращает копию.
 * 
 * Производит глубокое копирование.
 * При возникночении ошибок возвращает NULL-матрицу.
 */
matrix_t mat_copy(const matrix_t *matrix);

/**
 * @brief Изменяет размер матрицы, сохраняя
 * имеющиеся элементы в матрице.
 * 
 * Если была передана NULL-матрица, она инициализируется автоматически.
 * При возникновении ошибок не меняет исходную матрицу.
 * 
 * @return mat_bad_create - не удалось создать матрицу,
 *         mat_bad_alloc - не удалось выделить память для перемещения
 *                         элементов в новую область памяти,
 *         mat_success - успешное выполнение.
 */
int mat_resize(matrix_t *matrix, size_t new_rows, size_t new_cols);

/**
 * @brief Обменивает две строки матрицы.
 * 
 * При неверных индексах строк ничего не делает.
 */
void mat_swap_rows(matrix_t *matrix, size_t row_1, size_t row_2);

/**
 * @brief Прибавляет взвешенную строку матрицы к другой строке.
 * 
 * dest_row += k * src_row
 *
 * @param dest_row - индекс изменяемой строки,
 * @param src_row - индекс добавляемой строки,
 * @param k - коэффициент для умножения добавляемой строки.
 */
void mat_add_row(matrix_t *matrix, size_t dest_row, size_t src_row, matrix_elem_t k);

/**
 * @brief Вырезает из матрицы строку и столбец по переданным индексам.
 * 
 * @param reduce_row - индекс удаляемой строки,
 * @param reduce_row - индекс удаляемого столбца.
 */
matrix_t mat_reduced(const matrix_t *matrix, size_t reduce_row, size_t reduce_col);

/**
 * @brief Возвращает элемент, находящийся в заданной позиции.
 * 
 * При неверных параметрах возвращает 0.
 * 
 * @param row - индекс строки элемента,
 * @param col - индекс столбца элемента.
 */
matrix_elem_t mat_get(const matrix_t *matrix, size_t row, size_t col);

/**
 * @brief Устанавливает значение элемента,
 * находящегося в заданной позиции.
 * 
 * При неверных параметрах ничего не делает.
 * 
 * @param row - индекс строки элемента,
 * @param col - индекс столбца элемента.
 * @param value - устанавливаемое значение элемента.
 */
void mat_set(matrix_t *matrix, size_t row, size_t col, matrix_elem_t value);

/**
 * @brief Складывает две матрицы поэлементно.
 * 
 * @param mat_1 - первое слагаемое,
 * @param mat_2 - второе слагаемое,
 * @param res - матрица для помещения результата сложения.
 * 
 * @return mat_bad_matrix - переданы NULL-матрицы в качестве слагаемых,
 *         mat_bad_dims - размеры матриц не совпадают,
 *         mat_bad_create - не удалось создать матрицу,
 *         mat_bad_alloc - не удалось выделить память для перемещения
 *                         элементов в новую область памяти,
 *         mat_success - успешное выполнение.
 */
int mat_add(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res);

/**
 * @brief Умножает две матрицы по правилу матричного умножения.
 * 
 * @param mat_1 - первый множитель,
 * @param mat_2 - второй множитель,
 * @param res - матрица для помещения результата умножения.
 * 
 * @return mat_bad_matrix - переданы NULL-матрицы в качестве множителей,
 *         mat_bad_dims - размеры матриц не удовлетворяют умножению,
 *         mat_bad_create - не удалось создать матрицу,
 *         mat_bad_alloc - не удалось выделить память для перемещения
 *                         элементов в новую область памяти,
 *         mat_success - успешное выполнение.
 */
int mat_mult(const matrix_t *mat_1, const matrix_t *mat_2, matrix_t *res);

/**
 * @brief Решает систему линейных уравнений с
 * коэффициентами в матрице matrix размера N*(N+1).
 * 
 * @param matrix - матрица коэффициентов
 * @param result - матрица-столбец решений (если найдено)
 * 
 * @return mat_bad_dims - размер матрицы не удовлетворяют методу решения,
 *         mat_bad_create - не удалось создать промежуточную матрицу,
 *         mat_bad_alloc - не удалось выделить память для перемещения
 *                         элементов в новую область памяти,
 *         mat_bad_copy - неудачное копирование матрицы,
 *         mat_singular_matrix - переданная матрица коэффициентов
 *                               является вырожденной,
 *         mat_success - успешное выполнение.
 */
int mat_solve_sle(matrix_t *matrix, matrix_t *result);

#endif // __MATRIX_H_
