CC = gcc
CFLAGS = --std=c99 -Wall -Werror -pedantic

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

app.exe : dir $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

dir :
	mkdir -p $(OBJDIR)

$(OBJECTS) : $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY : clean
clean :
	rm -f ./**/*.o *.exe
