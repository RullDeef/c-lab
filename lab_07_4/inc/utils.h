#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Выводит массив чисел в файл
 * 
 * @param filename - имя файла для записи
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (элемент за последним)
 * 
 * @return  cant_open_file - невозможно открыть файл для записи
 *          success - успешное выполнение
 */
int write_file(char *filename, int *begin, int *end);

/**
 * @brief Считывает числа из файла в массив
 * 
 * Создает и возвращает динамический массив чисел
 * 
 * @param filename - название файла для чтения
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массива (на элемент за последним)
 * 
 * @return  cant_open_file - невозможно открыть файл
 *          invalid_elements_amount - неверная структура файла
 *          bad_alloc - неудачная попытка динамически выделить память
 *          success - успешное выполнение
 */
int read_data_file(char *filename, int **begin, int **end);

#endif
