CC = gcc
INCLUDE = inc
CFLAGS = --std=c99 -Wall -Werror -pedantic -I$(INCLUDE)

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY : clean
.NOTPARALLEL: release debug

release : CFLAGS+=-O3
release : app.exe

debug : CFLAGS+=--coverage -g
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
	rm -f **.o *.exe **.gcno **.gcda **.gcov
	rm -rf ./func_tests/gcov
