CFLAGS = -pedantic -Wall -Wextra -Wno-misleading-indentation -std=gnu99 -g
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj

all: ifj

ifj: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -D$(debug)DEBUG -c $^ -o $@

test:
	./tests/test.sh

clean:
	rm *.o $(PROGRAMS)
