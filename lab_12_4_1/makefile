CC := gcc

OSFLAG :=
LIBPREP :=
LIBEXT :=
APPEXT :=
ARR_LIB_PATH :=
LFLAGS :=
ifeq ($(OS),Windows_NT)
	APPEXT := exe
	LIBEXT := dll
	ARR_LIB_PATH := -D ARR_LIB_PATH=\"./bin/plugin/array.dll\"
	OSFLAG += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG += -D IA32
	endif
else
	APPEXT := out
	LIBPREP := lib
	LIBEXT := so
	ARR_LIB_PATH := -D ARR_LIB_PATH=\"./bin/plugin/libarray.so\"
	LFLAGS := -ldl
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

CFLAGS := -std=gnu99 -Wall -Werror -Wextra -Wno-cast-function-type $(OSFLAG) $(ARR_LIB_PATH)

SOURCES := $(shell find src -name *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
OBJECTS := $(subst src,obj,$(OBJECTS))

.PHONY : app-static app-dynamic app-plugin clean clean-obj

clean :
	rm -rf obj lib bin

clean-obj :
	rm -rf obj

app-static : bin/static/app.$(APPEXT)
	./bin/static/app.$(APPEXT)

app-dynamic : CFLAGS += -D ARRAY_DYNAMIC
app-dynamic : bin/dynamic/app.$(APPEXT)
	cp ./lib/$(LIBPREP)array.$(LIBEXT) ./bin/dynamic/$(LIBPREP)array.$(LIBEXT); ./bin/dynamic/app.$(APPEXT)

app-plugin : CFLAGS += -D ARRAY_DYNAMIC -D ARRAY_PLUGIN
app-plugin : bin/plugin/app.$(APPEXT)
	cp ./lib/$(LIBPREP)array.$(LIBEXT) ./bin/plugin/$(LIBPREP)array.$(LIBEXT); ./bin/plugin/app.$(APPEXT)

bin/static/app.$(APPEXT) : obj/main.o lib/libarray.a
	mkdir -p $(dir $@); $(CC) -o $@ obj/main.o -Llib -larray

bin/plugin/app.$(APPEXT) : obj/main.o lib/$(LIBPREP)array.$(LIBEXT)
	mkdir -p $(dir $@); $(CC) -o $@ obj/main.o $(LFLAGS)

bin/dynamic/app.$(APPEXT) : obj/main.o lib/$(LIBPREP)array.$(LIBEXT)
	mkdir -p $(dir $@); $(CC) -o $@ obj/main.o -Llib -larray

lib/libarray.a : obj/array.o
	mkdir -p $(dir $@); ar rcs $@ $^

lib/$(LIBPREP)array.$(LIBEXT) : obj/array.o
	mkdir -p $(dir $@); $(CC) -shared -o $@ $^

obj/array.o : src/array.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -fPIC -o $@ -c $^

obj/main.o : src/main.c
	mkdir -p $(dir $@); $(CC) $(CFLAGS) -o $@ -c $^
