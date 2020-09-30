#ifndef SORT_FUNC_H
#define SORT_FUNC_H

typedef int (*compar_fn_t)(const void *, const void *);

/**
 * @brief Сравнивает два целых числа
 * 
 * @return  отрицательное число - первое число меньше второго
 *          ноль - числа равны
 *          положительное число - первое число больше второго
 */
int int_comparator(const void *a, const void *b);

/**
 * @brief Сортирует массив по переданному компаратору
 * 
 * @param data_array - указатель на массив данных
 * @param num - кол-во элементов в массиве
 * @param size - размер одного элемента в байтах
 * @param comparator - сравнивающая функция
 * 
 * @return  invalid_args - некорректный указатель на массив данных
 *          invalid_elements_amount - некорректное число всех элементов
 *          invalid_elements_size - некорректный размер одного элемента
 *          success - успешное выполнение
 */
int mysort(void *data_array, int num, int size, compar_fn_t comparator);

#endif // SORT_FUNC_H
