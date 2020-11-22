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

unit : unit_tests.exe
	./unit_tests.exe

app.exe : obj/main.o \
		obj/item.o \
		obj/item_array.o \
		obj/item_io.o \
		obj/arg_reader.o
	$(CC) -o $@ $^ $(LFLAGS)

unit_tests.exe : LFLAGS += -lcheck -lpthread
unit_tests.exe : obj/check_main.o \
		obj/item.o \
		obj/item_array.o \
		obj/arg_reader.o \
		obj/check_item.o \
		obj/check_item_array.o \
		obj/check_arg_reader.o
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

obj/arg_reader.o : src/arg_reader.c \
		inc/arg_reader.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/main.o : src/main.c \
		inc/item.h \
		inc/item_array.h \
		inc/item_io.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/check_item.o : unit_tests/check_item.c \
		inc/item.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/check_item_array.o : unit_tests/check_item_array.c \
		inc/item_array.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/check_arg_reader.o : unit_tests/check_arg_reader.c \
		inc/arg_reader.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/check_main.o : unit_tests/check_main.c
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<
