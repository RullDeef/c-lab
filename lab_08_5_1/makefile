CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -g -ggdb --coverage
LFLAGS := -std=c99 --coverage

SRC := $(shell find ./src -name *.c)
OBJ := $(subst .c,.o,$(subst src,obj,$(SRC)))

.PHONY : clean fun unit

func : app.exe
	./func_tests/test.sh

unit : SRC -= ./src/main.c
unit : OBJ -= ./obj/main.o
unit : unit_tests.exe
	./unit_tests/test.sh

unit_tests.exe : $(SRC)
	$(CC) $(CFLAGS) -lcheck -lm -o $@ $^

app.exe : $(OBJ)
	$(CC) $(LFLAGS) -lm -o $@ $^

$(OBJ) : obj/%.o : src/%.c
	mkdir -p $(shell dirname $@); $(CC) $(CFLAGS) -c -o $@ $^

clean :
	rm -rf obj *.exe *.stackdump
