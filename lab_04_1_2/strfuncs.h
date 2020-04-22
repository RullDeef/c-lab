#ifndef STRFUNCS_H
#define STDFUNCS_H

#define MAX_STR_LEN (256 + 1)
#define MAX_WORD_LEN (16 + 1)
#define MAX_WORD_NUM (MAX_STR_LEN >> 1)

#define WORD_DELIM " .,?!:;-"

typedef char input_str_t[MAX_STR_LEN];
typedef char word_t[MAX_WORD_LEN];
typedef word_t word_array_t[MAX_WORD_NUM];

size_t my_strspn(const char *str1, const char_t *str2);
size_t my_strcspn(const char *str1, const char_t *str2);
int split_str(const input_str_t str, word_array_t word_array, size_t *size);
void word_assign(word_t dest, const word_t src);
void swap_word(word_t word1, word_t word2);

#endif
