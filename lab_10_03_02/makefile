CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -extra -O0 -g -ggdb -Iinc
LFLAGS := --coverage

SOURCES := $(shell find ./src -type f -name *.c ! -name main.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

CHECK_SRCS := $(shell find ./unit -type f -name *.c)
CHECK_OBJS := $(subst .c,.o,$(CHECK_SRCS))
CHECK_OBJS := $(subst unit,obj,$(CHECK_OBJS))

.PHONY : run clean func

func : app.exe
func :
	py ./func/test.py

unit : unit_tests.exe
	./unit_tests.exe

run : app.exe
	./app.exe

unit_tests.exe : $(OBJECTS) $(CHECK_OBJS)
	$(CC) $(LFLAGS) -lcheck -o $@ $^

app.exe : $(OBJECTS) obj/main.o
	$(CC) $(LFLAGS) -o $@ $^

$(OBJECTS) obj/main.o : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^

$(CHECK_OBJS) : obj/%.o : unit/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^

clean :
	rm -rf obj *.exe
