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

debug: part1.c part2.c
	$(CC) part1.c $(CFLAGS) $(DBGCFLAGS) -o part1_dbg
	$(CC) part2.c $(CFLAGS) $(DBGCFLAGS) -o part2_dbg

release: part1.c part2.c
	$(CC) part1.c $(CFLAGS) $(RELCFLAGS) -o part1
	$(CC) part2.c $(CFLAGS) $(RELCFLAGS) -o part2

clean:
	rm -f part1 part1_dbg
	rm -f part2 part2_dbg

all: release
