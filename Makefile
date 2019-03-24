CC=gcc
SH=bash

CFLAGS=-Wall -g3

TARGET=list
INCLUDES=-I./containers/list/ -I./tools/ -I./unit_tests
HEADERS=./containers/list/list.h \
		./tools/status.h \
		./tools/logger.h
SOURCES=./containers/list/list.c \
		./tools/status.c

UT_TARGET=unit_test
UT_FILES=./unit_tests/test_list.c \
		 ./unit_tests/test_list.h

all: code test

code: $(TARGET)

test: $(UT_TARGET)

$(TARGET): $(HEADERS) $(SOURCES)
	$(CC) $(INCLUDES) $(CFLAGS) $(HEADERS) $(SOURCES) main.c -o $@

$(UT_TARGET): $(HEADERS) $(SOURCES) $(UT_FILES)
	$(CC) $(INCLUDES) $(CFLAGS) $(HEADERS) $(SOURCES) $(UT_FILES) -o $@

$(COV_TARGET)

clean:
	rm $(TARGET)
	rm $(UT_TARGET)
