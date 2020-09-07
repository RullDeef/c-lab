#ifndef _ITEM_H_
#define _ITEM_H_

#include <stdbool.h>

#define MAX_ITEM_NAME_SIZE 26

typedef struct
{
    char name[MAX_ITEM_NAME_SIZE];
    float mass;
    float volume;
    float density;
} item_t;

/**
 *  @brief Проверяет, содержит ли массив некорректные структуры.
 *  
 *  Структура считается некорректной, если она имеет
 *  неположительные поля mass и/или volume.
 *  
 *  @param items - массив для проверки.
 *  @param items_count - кол-во элементов массива.
 *  
 *  @return true - массив содержит некорректные структуры,
 *          false - все структуры масива корректны.
 */
bool has_invalid_items(const item_t *items, short int items_count);

/**
 *  @brief Вычисляет плотность каждого предмета в массиве.
 *  
 *  Не делает никаких дополнительных проверок
 *  на корректность структур.
 *  
 *  @param items - массив для вычислений.
 *  @param items_count - кол-во элементов массива.
 */
void calculate_density(item_t *items, short int items_count);

/**
 *  @brief Сортирует предметы по возрастанию их плотности.
 *  
 *  Не производит никаких дополнительных проверок и
 *  расчетов. Использует аналог алгоритма сортировки "пузырьком".
 *  
 *  @param items - массив для сортировки.
 *  @param items_count - кол-во элементов массива.
 */
void sort_items_array(item_t *items, short int items_count);

/**
 *  @brief Фильтрует массив, оставляя элементы, название которых
 *  начинается с prefix.
 *  
 *  @param items - массив для проверки.
 *  @param items_count - кол-во элементов массива.
 *  @param prefix - подстрока для сравнения имен предметов.
 */
void filter_array(item_t *items, short int *items_count, const char *prefix);

#endif // _ITEM_H_
