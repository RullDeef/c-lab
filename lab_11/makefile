CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -extra -Iinc

unit : unit.exe
	./unit.exe

unit.exe : unit/check_strlib.c src/strlib.c inc/strlib.h
	$(CC) $(CFLAGS) -o $@ src/strlib.c unit/check_strlib.c
