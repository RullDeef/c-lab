CC := gcc

OSFLAG :=
ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG += -D OSX
	endif
		UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		OSFLAG += -D AMD64
	endif
		ifneq ($(filter %86,$(UNAME_P)),)
	OSFLAG += -D IA32
		endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		OSFLAG += -D ARM
	endif
endif

CFLAGS := -std=gnu99 -Wall -Werror -pedantic -Wextra -Wno-cast-function-type $(OSFLAG)

SOURCES := $(shell find src -name *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

.PHONY : app-static app-dynamic app-plugin clean clean-obj

clean :
	rm -rf obj lib bin

clean-obj :
	rm -rf obj

app-static : bin/static/app.exe
	./bin/static/app.exe

app-dynamic : CFLAGS += -D ARRAY_DYNAMIC
app-dynamic : bin/dynamic/app.exe
	cp ./lib/array.dll ./bin/dynamic/array.dll; ./bin/dynamic/app.exe

app-plugin : CFLAGS += -D ARRAY_DYNAMIC -D ARRAY_PLUGIN
app-plugin : bin/plugin/app.exe
	cp ./lib/array.dll ./bin/plugin/array.dll; ./bin/plugin/app.exe

bin/static/app.exe : obj/main.o lib/libarray.a
	mkdir -p $(dir $@); $(CC) -o $@ obj/main.o -Llib -larray

bin/plugin/app.exe : obj/main.o lib/array.dll
	mkdir -p $(dir $@); $(CC) -o $@ obj/main.o

bin/dynamic/app.exe : obj/main.o
	mkdir -p $(dir $@); $(CC) -o $@ obj/main.o -Llib -larray

lib/libarray.a : obj/array.o
	mkdir -p $(dir $@); ar rcs $@ $^

lib/array.dll : obj/array.o
	mkdir -p $(dir $@); $(CC) -shared -o $@ $^

obj/array.o : src/array.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -fPIC -o $@ -c $^

obj/main.o : src/main.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
