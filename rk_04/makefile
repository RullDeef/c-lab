CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra \
	-Wno-int-to-pointer-cast -Wno-pointer-to-int-cast \
	-Wno-cast-function-type -Isrc

SOURCES := $(shell find src -name *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

.PHONY : run clean

run : app.exe
	./app.exe

clean :
	rm -rf obj app.exe

app.exe : $(OBJECTS)
	$(CC) -o $@ $^

$(OBJECTS) : obj/%.o : src/%.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
