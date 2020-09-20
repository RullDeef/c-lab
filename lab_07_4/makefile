CC = gcc
INCLUDE = inc
CFLAGS = --std=c99 -Wall -Werror -pedantic -I$(INCLUDE)
LDFLAGS = -lcheck -lpthread -lrt

SRCDIR = src
OBJDIR = out

TEST_SRCDIR = .
TEST_OBJDIR = out

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

TEST_SOURCES = $(wildcard $(TEST_SRCDIR)/*.c) $(wildcard $(SRCDIR)/*_func.c)
TEST_SOURCES-= $(TEST_SRCDIR)/main.c
TEST_OBJECTS = $(patsubst $(TEST_SRCDIR)/%.c,$(TEST_OBJDIR)/%.o,$(TEST_SOURCES))

.PHONY : clean func unit
.NOTPARALLEL: release debug

release : CFLAGS+= -O3
release : app.exe

debug : CFLAGS+= --coverage -g -ggdb -fno-inline -fno-omit-frame-pointer
debug : app.exe

app.exe : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

test.exe : $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(TEST_OBJECTS)

$(OBJECTS) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR); $(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJECTS) : $(TEST_OBJDIR)/%.o : $(TEST_SRCDIR)/%.c
	mkdir -p $(OBJDIR); $(CC) $(CFLAGS) -c $< -o $@

func : app.exe
	./func_tests/func_test.sh

unit : test.exe
	./test.exe

clean :
	rm -f *.exe **/*.gcno **/*.gcda
	rm -rf ./func_tests/gcov $(OBJDIR) $(TEST_OBJDIR)
