# Makefile created by Pavel Holica
# This Makefile was created for _testing purposes only_. 
# Homework solution is (ant won't be) provided with this Makefile.
# According to pb071 rules, this does not violate any rule.

CC=gcc
# GDB and DWARF content is included for debugging purposes
CFLAGS=-std=c99 -pedantic -Wall -Wextra -O2
#CFLAGS=-std=c99 -pedantic -Wall -Wextra -O2 -DDEBUG -ggdb3 -gdwarf-2

program=maze

OBJ=main.o logic.o saveload.o
SRC=main.c logic.c saveload.c
HEAD=logic.h saveload.h

.PHONY: build
.PHONY: run
.PHONY: test
.PHONY: clean

build: ${program}

test: ${program}
	./${program} ./data/testmaze.txt

run: ${program}
	echo "program has to be run manually"

clean:
	rm -f ${OBJ} ${program}

${program}: ${OBJ}
	${CC} ${OBJ} -o ${program} ${CFLAGS} -lncurses

${OBJ}: ${HEAD}
