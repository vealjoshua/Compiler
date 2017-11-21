CC=gcc
CFLAGS=-I.
DEPS = scanner.h testScanner.h token.h
OBJ = main.o testScanner.o scanner.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

p1: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)