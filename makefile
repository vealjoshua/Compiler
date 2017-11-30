CC = gcc
CFLAGS = -g -I.
DEPS = extern.h parser.h scanner.h stack.h testTree.h token.h tree.h
OBJ = main.o parser.o scanner.o stack.o testTree.o token.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

p3: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)