# Makefile for Files Protection Program

CC = gcc

prefix = /usr/local/bin

BIN_FILE = fpp_client

SRC_FILES += main.c
SRC_FILES += fpp.c
SRC_FILES += io_binary_streams.c
SRC_FILES += io_text_streams.c
SRC_FILES += progress_bar.c
SRC_FILES += rsa.c

CFLAGS  = -Iinc -Wall -static -O3 -s
LDFLAGS = -lgmp

OBJ_FILES := $(patsubst %.c,obj/%.o,$(SRC_FILES))
QUIET_CC = @echo '   ' CC $(notdir $@);

.ONESHELL:
.PHONY: build


all: mkdirs build

# test: mkdirs debug

build: $(OBJ_FILES)
	$(CC) $^ -o bin/$(BIN_FILE) $(CFLAGS) $(LDFLAGS)

obj/%.o:  src/%.c
	$(QUIET_CC)$(CC) -c $< -o $@ $(CFLAGS)

install: mkdirs build
	install bin/$(BIN_FILE) $(prefix)

uninstall:
	rm -f $(prefix)/$(BIN_FILE)

clean:
	rm -f obj/*.o

mkdirs: 
	@if [ ! -d bin ] ; then
		mkdir bin
	fi

	@if [ ! -d obj ] ; then
		mkdir obj
	fi
