CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic
LFLAGS := -std=c99 --coverage

.PHONY : clean release func_test unit_test

release : CFLAGS += -O3
release : app.exe

func_test : CFLAGS += -O0 -g -ggdb --coverage
func_test : app.exe
	./func_tests/test.sh

unit_test : unit_tests.exe
	cd ./unit_tests/data
	# valgrind -q ../../unit_tests.exe

unit_tests.exe : \
		unit_tests/src/check_main.c \
		unit_tests/src/matrix/check_matrix.c \
		unit_tests/src/io/check_mat_io.c \
		src/matrix/matrix.c \
		src/io/mat_io.c
	$(CC) $(CFLAGS) -lcheck -lm -o $@ $^

app.exe : \
		obj/main.o \
		obj/app_params.o \
		obj/mat_io.o \
		obj/matrix.o
	$(CC) $(LFLAGS) -lm -o $@ $^

obj/main.o : src/main.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

obj/app_params.o : src/io/app_params.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

obj/mat_io.o : src/io/mat_io.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

obj/matrix.o : src/matrix/matrix.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

clean :
	rm -rf ./obj *.exe *.stackdump ./func_tests/gcov ./func_tests/out
	find . -type f -name "*.gcda" -delete
	find . -type f -name "*.gcno" -delete
	find . -type f -name "*.c.gcov" -delete
