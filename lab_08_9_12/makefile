CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -g -ggdb --coverage
LFLAGS := -std=c99 --coverage

.PHONY : clean test
.NOTPARALLEL : app.exe test

app.exe : obj/main.o obj/app_params.o obj/mat_io.o obj/matrix.o
	$(CC) $(LFLAGS) -o $@ $^

obj/main.o : src/main.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

obj/app_params.o : src/io/app_params.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

obj/mat_io.o : src/io/mat_io.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

obj/matrix.o : src/matrix/matrix.c
	mkdir -p obj; $(CC) $(CFLAGS) -c -o $@ $^

test : app.exe
	./func_tests/test.sh

clean :
	rm -rf ./obj *.exe *.stackdump ./func_tests/gcov
	find . -type f -name "*.gcda" -delete
	find . -type f -name "*.gcno" -delete
	find . -type f -name "*.c.gcov" -delete
