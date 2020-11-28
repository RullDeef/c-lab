CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -extra -g3 -O0 -g -ggdb -Iinc
LFLAGS := --coverage

SOURCES := $(shell find ./src -type f -name *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

CHECK_SRC := $(shell find ./unit -type f -name *.c)
CHECK_OBJS := $(subst .c,.o,$(CHECK_SRC))
CHECK_OBJS := $(subst unit,obj,$(CHECK_OBJS))

.PHONY : unit clean

unit : unit_tests.exe
	valgrind --leak-check=full -- ./unit_tests.exe

clean :
	rm -rf obj *.exe

unit_tests.exe : $(CHECK_OBJS) $(OBJECTS)
	$(CC) -o $@ $^ -lcheck

$(OBJECTS) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^

$(CHECK_OBJS) : obj/%.o : unit/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
