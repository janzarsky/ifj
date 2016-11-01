CFLAGS = -pedantic -Wall -Wextra
SOURCES = $(filter-out parser_test.c, $(wildcard *.c))
OBJECTS = $(SOURCES:.c=.o)

all: ifj parser_test

parser_test: parser_test.c
	gcc $(CFLAGS) $^ -o $@

ifj: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
