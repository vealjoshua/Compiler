CC = gcc
CFLAGS = -g -I.
DEPS = extern.h parser.h scanner.h testTree.h token.h tree.h
OBJ = main.o parser.o scanner.o testTree.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

p2: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)