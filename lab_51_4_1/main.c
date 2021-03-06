#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <inttypes.h>

#define NUM_CHUNK_LENGTH 3

typedef enum
{
    exit_success = 0,
    end_of_input,
    invalid_input,
    too_few_numbers
} status_code_t;

status_code_t get_next_number(int numbers[NUM_CHUNK_LENGTH])
{
    status_code_t status_code;

    // shift previous numbers in front
    for (size_t i = 0; i + 1 < NUM_CHUNK_LENGTH; i++)
        numbers[i] = numbers[i + 1];
    
    // try to read and push back new number
    int readed = fscanf(stdin, "%d", &numbers[NUM_CHUNK_LENGTH - 1]);
    
    if (readed == 1)
        status_code = exit_success;
    else
        status_code = end_of_input;

    return status_code;
}

bool mid_is_maximum(const int numbers[NUM_CHUNK_LENGTH])
{
    return numbers[0] < numbers[1] && numbers[1] > numbers[2];
}

int main(void)
{
    int numbers[NUM_CHUNK_LENGTH] = { 0, 0, 0 };
    int8_t readed_chunk_length = 0;
    size_t maximums_amount = 0;

    status_code_t status_code = exit_success;

    while (true)
    {
        status_code = get_next_number(numbers);
        if (status_code != exit_success)
            break;

        // инкрементируем счётчик только пока имеет место быть ошибка
        // связанная с недостаточным количеством чисел для анализа
        readed_chunk_length++;
        if (readed_chunk_length > NUM_CHUNK_LENGTH)
            readed_chunk_length--;
        
        // если чисел достаточно для обработки, то обрабатываем
        if (readed_chunk_length == NUM_CHUNK_LENGTH)
            maximums_amount += (size_t)mid_is_maximum(numbers);
    }

    if (readed_chunk_length < NUM_CHUNK_LENGTH)
        status_code = too_few_numbers;
    else if (status_code == end_of_input)
    {
        fprintf(stdout, "%zu", maximums_amount);
        status_code = exit_success;
    }
    
    return status_code;
}
