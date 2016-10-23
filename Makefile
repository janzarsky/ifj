CFLAGS = -pedantic -Wall -Wextra
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

all: ifj

ifj: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
