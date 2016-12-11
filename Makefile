CFLAGS = -pedantic -Wall -Wextra -std=gnu99
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj

all: ifj

ifj: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
