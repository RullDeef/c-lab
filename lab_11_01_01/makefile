CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Iinc
LFLAGS := -lcheck

.PHONY : unit clean

unit : unit_tests.exe
	./unit_tests.exe

unit_tests.exe : src/strlib.c unit/check_strlib.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean :
	rm -rf *.exe
