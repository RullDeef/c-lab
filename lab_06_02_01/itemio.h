#ifndef _ITEMIO_H_
#define _ITEMIO_H_

#include "status_codes.h"
#include "item.h"

#define MAX_ITEMS_AMOUNT 15

/**
 *  @brief Считывает массив предметов из файла.
 *  
 *  @param filename - название файла для чтения.
 *  @param items - выход для считываемого массива.
 *  @param items_count - размер считанного массива.
 * 
 *  @return success - считывание произошло успешно,
 *          cant_open_file - невозможно открыть файл,
 *          max_items_amount_reached - слишком много структур в файле.
 */
status_code_t read_items(const char *filename, item_t *items, short int *items_count);

/**
 *  @brief Выводит массив предметов на экран.
 *  
 *  @param items - массив для вывода.
 *  @param items_count - кол-во элементов массива.
 */
void print_array(item_t *items, short int items_count);

#endif // _ITEMIO_H_
