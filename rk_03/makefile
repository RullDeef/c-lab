CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -extra -g3 -O0 -Iinc
LFLAGS := -lcurses

SOURCES := $(shell find ./src -type f -name *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

.PHONY : build run clean

build : app.exe

run : app.exe
	./app.exe

clean :
	rm -rf obj app.exe

app.exe : $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJECTS) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
