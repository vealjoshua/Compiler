CC = gcc
CFLAGS = -g -I.
DEPS = codeGenerator.h extern.h parser.h scanner.h stack.h staticSemantics.h token.h tree.h
OBJ = codeGenerator.o main.o parser.o scanner.o stack.o staticSemantics.o token.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

comp: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)