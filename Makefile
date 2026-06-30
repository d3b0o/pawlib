NAME    := pawlib
CC      := gcc
CFLAGS  := -Iinclude -Isrc -Wall -Wextra

SRCS    := $(wildcard src/*.c)
PREFIX  := /usr/local

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
    LIB     := lib$(NAME).dylib
    LDFLAGS := -dynamiclib
    LDCACHE :=
else
    LIB     := lib$(NAME).so
    LDFLAGS := -shared -fPIC
    LDCACHE := ldconfig
endif


all: $(LIB)

$(LIB): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

install: $(LIB)
	install -d $(PREFIX)/include $(PREFIX)/lib
	install -m 644 include/$(NAME).h $(PREFIX)/include/
	install -m 755 $(LIB)            $(PREFIX)/lib/
	$(LDCACHE)

example: $(LIB)
	$(CC) $(CFLAGS) template/example.c -o example -L. -l$(NAME) -Wl,-rpath,'$$ORIGIN'
	./example

clean:
	rm -f $(LIB) example

.PHONY: all install example clean
