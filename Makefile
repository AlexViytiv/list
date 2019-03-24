CC=gcc

CFLAGS=-Wall -g3

HEADERS=./containers/list/list.h ./tools/status.h ./tools/logger.h
SOURCES=./containers/list/list.c ./tools/status.c main.c

all: list

list: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(HEADERS) $(SOURCES) -o $@

