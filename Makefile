CFLAGS = -pedantic -Wall -Wextra
PROGRAMS = ifj

all: $(PROGRAMS)

ifj: main.o instrlist.o
	gcc $(CFLAGS) $^ -o $@

main.o: main.c
	gcc $(CFLAGS) -c $^ -o $@

instrlist.o: instrlist.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm *.o $(PROGRAMS)
