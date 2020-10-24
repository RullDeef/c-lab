CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Iinc
LFLAGS :=

.PHONY : clean func unit

clean :
	rm -rf obj *.exe

func : CFLAGS += -O0 -g -ggdb --coverage
func : LFLAGS += --coverage
func : app.exe
	./func_tests/test.sh

unit_tests.exe :
	echo unit

app.exe : \
		obj/item.o \
		obj/item_array.o \
		obj/item_io.o \
		obj/main.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/item.o : src/item.c \
		inc/item.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/item_array.o : src/item_array.c \
		inc/item.h \
		inc/item_array.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/item_io.o : src/item_io.c \
		inc/item.h \
		inc/item_array.h \
		inc/item_io.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/main.o : src/main.c \
		inc/item.h \
		inc/item_array.h \
		inc/item_io.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<
