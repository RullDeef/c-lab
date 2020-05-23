#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_FILE_NAME "in.txt"
#define OUTPUT_FILE_NAME "out.txt"

#define MAX_CITY_NAME_LENGTH (100 + 3)
#define MAX_YEAR_LENGTH (4 + 3)
#define MAX_POPULATION_LENGTH (10 + 3)

typedef enum
{
    EXIT_GOOD = 0,
    UNABLE_TO_OPEN_INPUT_FILE,
    UNABLE_TO_OPEN_OUTPUT_FILE,
    NO_CITIES_READED,
    END_OF_FILE,
    UNABLE_TO_WRITE_OUTPUT,

    UNABLE_TO_READ_CITY_NAME,
    UNABLE_TO_READ_CITY_YEAR,
    UNABLE_TO_READ_CITY_POPULATION
} status_code_t;

typedef struct
{
    char name[MAX_CITY_NAME_LENGTH];
    int year;
    long long population;
} city_t;

status_code_t open_files(FILE **fin, FILE **fout);
status_code_t process(FILE *fin, FILE *fout);
status_code_t close_files(FILE *fin, FILE *fout);

status_code_t get_population_of_longest_city(FILE *fin, long long *population);
status_code_t get_average_population(FILE *fin, long long *population);
status_code_t write_difference(FILE *fout, long long population_1, long long population_2);

status_code_t read_next_city(FILE *fin, city_t *city);
bool city_is_longer(city_t city_1, city_t city_2);

int main(void)
{
    FILE *fin;
    FILE *fout;
    status_code_t status_code = EXIT_GOOD;

    status_code = open_files(&fin, &fout);
    if (status_code == EXIT_GOOD)
        status_code = process(fin, fout);
    close_files(fin, fout);

    return status_code;
}

status_code_t open_files(FILE **fin, FILE **fout)
{
    status_code_t status_code = EXIT_GOOD;

    *fin = fopen(INPUT_FILE_NAME, "rt");
    if (*fin != NULL)
    {
        *fout = fopen(OUTPUT_FILE_NAME, "wt");
        if (*fout == NULL)
            status_code = UNABLE_TO_OPEN_OUTPUT_FILE;
    }
    else
        status_code = UNABLE_TO_OPEN_INPUT_FILE;

    return status_code;
}

status_code_t process(FILE *fin, FILE *fout)
{
    long long longest_city_population;
    long long average_population;
    status_code_t status_code = EXIT_GOOD;

    status_code = get_population_of_longest_city(fin, &longest_city_population);
    if (status_code == EXIT_GOOD)
    {
        status_code = get_average_population(fin, &average_population);
        if (status_code == EXIT_GOOD)
            status_code = write_difference(fout, longest_city_population, average_population);
    }

    return status_code;
}

status_code_t close_files(FILE *fin, FILE *fout)
{
    if (fin != NULL)
        fclose(fin);
    if (fout != NULL)
        fclose(fout);
}

status_code_t get_population_of_longest_city(FILE *fin, long long *population)
{
    city_t current_city;
    city_t longest_city;
    bool any_city_readed = false;

    status_code_t status_code = EXIT_GOOD;

    fseek(fin, 0L, SEEK_SET);
    *population = 0L;

    while ((status_code = read_next_city(fin, &current_city)) == EXIT_GOOD)
    {
        if (!any_city_readed)
        {
            longest_city = current_city;
            any_city_readed = true;
        }
        else if (city_is_longer(current_city, longest_city))
        {
            longest_city = current_city;
            *population = longest_city.population;
        }
    }

    if (!any_city_readed)
        status_code = NO_CITIES_READED;
    
    else if (status_code == END_OF_FILE)
        status_code = EXIT_GOOD;

    return status_code;
}

status_code_t get_average_population(FILE *fin, long long *population)
{
    city_t city;
    size_t cities_amount = 0;
    long double precise_population = 0.0;
    status_code_t status_code = EXIT_GOOD;

    fseek(fin, 0L, SEEK_SET);

    while ((status_code = read_next_city(fin, &city)) == EXIT_GOOD)
    {
        precise_population += (long double)city.population;
        cities_amount++;
    }

    if (cities_amount == 0)
        status_code = NO_CITIES_READED;

    else if (status_code == END_OF_FILE)
        status_code = EXIT_GOOD;

    *population = precise_population / cities_amount;
    return status_code;
}

status_code_t write_difference(FILE *fout, long long population_1, long long population_2)
{
    long long diff = population_1 - population_2;
    int wroted = fprintf(fout, "%lld", diff);

    if (wroted != 1)
        return UNABLE_TO_WRITE_OUTPUT;
    
    return EXIT_GOOD;
}

status_code_t read_next_city(FILE *fin, city_t *city)
{
    status_code_t status_code = EXIT_GOOD;
    char year_str[MAX_YEAR_LENGTH] = {};
    char population_str[MAX_POPULATION_LENGTH] = {};

    char *res = fgets(city->name, MAX_CITY_NAME_LENGTH, fin);
    if (res != NULL)
    {
        // do not delete endl char - it does not affect on results
        res = fgets(year_str, MAX_YEAR_LENGTH, fin);
        if (res != NULL)
        {
            city->year = atoi(year_str);
            res = fgets(population_str, MAX_POPULATION_LENGTH, fin);
            if (res != NULL)
                city->population = atoll(population_str);
            else
                status_code = UNABLE_TO_READ_CITY_POPULATION;
        }
        else
            status_code = UNABLE_TO_READ_CITY_YEAR;
    }
    else
        status_code = END_OF_FILE;

    return status_code;
}

bool city_is_longer(city_t city_1, city_t city_2)
{
    size_t name_len_1 = strlen(city_1.name);
    size_t name_len_2 = strlen(city_2.name);
    return name_len_1 > name_len_2;
}
