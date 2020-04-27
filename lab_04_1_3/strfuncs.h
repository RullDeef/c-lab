#ifndef STRFUNCS_H
#define STRFUNCS_H

#define MAX_STR_LEN (256 + 1)
#define MAX_WORD_LEN (16 + 1)
#define MAX_WORD_NUM (MAX_STR_LEN >> 1)

typedef char word_t[MAX_WORD_LEN];
typedef word_t word_array_t[MAX_WORD_NUM];

int input_str(char *str);
int split_str(const char *str, word_array_t word_array, size_t *size);
int remove_same_as_last(word_array_t word_array, size_t *size);
void remove_repititions(word_t word);
void reverse(word_array_t word_array, const size_t size);
void output(word_array_t word_array, const size_t size);

#endif
