CC=gcc

CFLAGS=-Wall -g3

TARGET=list
HEADERS=./containers/list/list.h ./tools/status.h ./tools/logger.h
SOURCES=./containers/list/list.c ./tools/status.c main.c

all: $(TARGET)

$(TARGET): $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(HEADERS) $(SOURCES) -o $@

clean:
	rm $(TARGET)



