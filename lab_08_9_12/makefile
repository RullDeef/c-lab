CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -g -ggdb
LFLAGS :=

.PHONY : clean

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

clean :
	rm -rf obj *.exe
