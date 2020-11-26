CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Iinc
LFLAGS := -lcheck

UNIT_SRC := $(shell find ./unit -name *.c)

.PHONY : unit clean

unit : unit_tests.exe
	./unit_tests.exe

unit_tests.exe : src/strlib.c $(UNIT_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean :
	rm -rf *.exe
