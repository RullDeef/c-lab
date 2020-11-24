CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -g -ggdb
LFLAGS := -lm

SRC := $(shell find ./src -name *.c)
OBJ := $(subst .c,.o,$(subst src,obj,$(SRC)))

.PHONY : clean fun unit

func : app.exe
	./func_tests/test.sh

unit : unit_tests.exe
	./unit_tests.exe

unit_tests.exe : ./unit/check_main.c
	$(CC) $(CFLAGS) -o $@ $^

app.exe : $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ) : obj/%.o : src/%.c
	mkdir -p $(shell dirname $@); $(CC) $(CFLAGS) -c -o $@ $^

clean :
	rm -rf obj *.exe *.stackdump
