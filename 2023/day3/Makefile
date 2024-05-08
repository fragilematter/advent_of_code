#
# Compiler flags
#
CC     = gcc
CFLAGS = -Wall -Werror -Wextra -lm

#
# Release build settings
#
RELCFLAGS = -O3

#
# Debug build settings
#
DBGCFLAGS = -g -O0 -DDEBUG

.PHONY: all clean debug release

debug: part1.c
	$(CC) part1.c $(CFLAGS) $(DBGCFLAGS) -o part1_dbg

release: part1.c
	$(CC) part1.c $(CFLAGS) $(RELCFLAGS) -o part1

clean:
	rm -f part1 part1_dbg

all: release
