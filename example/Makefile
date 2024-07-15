SOURCE_PATH_SIZE := $(shell printf $$(pwd)/ | wc -c)

CC=gcc
CPPFLAGS=-I../include/
# CPPFLAGS += -DSOURCE_PATH_SIZE=$(SOURCE_PATH_SIZE)
CFLAGS=-Wall -Werror -Wextra -Wvla -pedantic -std=c11 -g -O0
LDLIBS=-L../build/ -layaztub
LDFLAGS=-Wl,-rpath,../build/

all: main

main: main.o

clean:
	${RM} main main.o

.PHONY: all clean
