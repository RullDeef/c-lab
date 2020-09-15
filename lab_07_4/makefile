CC = i686-w64-mingw32-gcc.exe
INCLUDE = inc
CFLAGS = --std=c99 -static-libgcc -Wall -Werror -pedantic -I$(INCLUDE)

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY : clean
.NOTPARALLEL: release debug

release : CFLAGS+=-O3
release : app.exe

debug : CFLAGS+=--coverage -g -ggdb -fno-inline -fno-omit-frame-pointer
debug : app.exe tests.exe
	./func_tests/func_test.sh

app.exe : dir $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

tests.exe : dir $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

dir :
	mkdir -p $(OBJDIR)

$(OBJECTS) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f **/*.o *.exe **/*.gcno **/*.gcda
	rm -rf ./func_tests/gcov
