CFLAGS = -pedantic -Wall -Wextra -Wno-misleading-indentation -std=gnu99 -g
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj

all: ifj

#expr_parser_test: expr_parser_test.c expr_parser.c instrlist.c
#	gcc $(CFLAGS) -DDEBUG $^ -o $@

ifj: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -DDEBUG -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
