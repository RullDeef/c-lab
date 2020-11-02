CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -extra -O0 -g -ggdb -Iinc
LFLAGS := --coverage

SOURCES := $(shell find ./src -type f -name *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

.PHONY : run clean func

func : app.exe
func :
	py ./func/test.py

run : app.exe
	./app.exe

app.exe : $(OBJECTS)
	$(CC) $(LFLAGS) -o $@ $^

$(OBJECTS) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^

clean :
	rm -rf obj *.exe
