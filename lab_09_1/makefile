CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Iinc
LFLAGS :=

.PHONY : clean

clean :
	rm -rf obj *.exe

app.exe : \
		obj/film.o \
		obj/film_array.o \
		obj/reader.o \
		obj/printer.o \
		obj/main.o
	$(CC) -o $@ $^ $(LFLAGS)

obj/film.o : src/film.c \
		inc/film.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/film_array.o : src/film_array.c \
		inc/film.h \
		inc/film_array.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/reader.o : src/reader.c \
		inc/film.h \
		inc/film_array.h \
		inc/reader.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/printer.o : src/printer.c \
		inc/film.h \
		inc/film_array.h \
		inc/printer.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<

obj/main.o : src/main.c \
		inc/film.h \
		inc/film_array.h \
		inc/reader.h
	mkdir -p obj; $(CC) $(CFLAGS) -o $@ -c $<
