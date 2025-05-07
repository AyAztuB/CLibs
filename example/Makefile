SOURCE_PATH_SIZE := $(shell printf $$(pwd)/ | wc -c)

CC=gcc
CPPFLAGS=-I../include/
# CPPFLAGS += -DSOURCE_PATH_SIZE=$(SOURCE_PATH_SIZE)
CFLAGS=-Wall -Werror -Wextra -Wvla -pedantic -std=c11 -g -O0 -Wno-attributes
LDLIBS=-L../build/ -layaztub -lpthread
LDFLAGS=-Wl,-rpath,../build/

EXAMPLE_BINS = \
	example_logger \
	example_assert \
	example_debug \
	example_atomics \


all: ${EXAMPLE_BINS}

clean:
	${RM} ${EXAMPLE_BINS}

.PHONY: all clean
