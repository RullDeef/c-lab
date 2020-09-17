CC = gcc
INCLUDE = inc
CFLAGS = --std=c99 -Wall -Werror -pedantic -I. -I$(INCLUDE)

SRCDIR = src
OBJDIR = obj

TEST_SRCDIR = tests
TEST_OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

TEST_SOURCES = $(wildcard $(TEST_SRCDIR)/*.c) $(wildcard $(SRCDIR)/*_func.c)
TEST_OBJECTS = $(patsubst $(TEST_SRCDIR)/%.c,$(TEST_OBJDIR)/%.o,$(TEST_SOURCES))

.PHONY : clean
.NOTPARALLEL: release debug

release : CFLAGS+=-O3
release : app.exe

debug : CFLAGS+=--coverage -g -ggdb -fno-inline -fno-omit-frame-pointer
debug : app.exe tests.exe
	./tests.exe
	# ./func_tests/func_test.sh

app.exe : dir $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

tests.exe : dir $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -lcheck -o $@ $(TEST_OBJECTS)

dir :
	mkdir -p $(OBJDIR)

$(OBJECTS) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(TEST_OBJECTS) : $(TEST_OBJDIR)/%.o : $(TEST_SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean :
	rm -f **/*.o *.exe **/*.gcno **/*.gcda
	rm -rf ./func_tests/gcov
