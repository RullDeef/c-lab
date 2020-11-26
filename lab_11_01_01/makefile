CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -extra -Iinc

.PHONY : unit clean

unit : unit_tests.exe
	./unit_tests.exe

unit_tests.exe : unit/check_strlib.c src/strlib.c inc/strlib.h
	$(CC) $(CFLAGS) -o $@ src/strlib.c unit/check_strlib.c

clean :
	rm -rf *.exe
