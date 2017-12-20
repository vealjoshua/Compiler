#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "extern.h"
#include "parser.h"
#include "testTree.h"
#include "tree.h"

extern FILE* inputFile;
extern FILE* outputFile;

void processArgs(int argc, char* argv[]);
char *extractFileExtension(char *file);

int main(int argc, char* argv[])
{
	// Handle command line arguments
	processArgs(argc, argv);

	// Get root node from parse tree
	node* root = parser();
	fclose(inputFile);
	if (argc == 2)
		remove("temp.txt");

	startStaticSemantics(root);
	fclose(outputFile);
	deleteTree(root);
	return 0;
}

void processArgs(int argc, char* argv[])
{
	if (argc <1)
	{
		fprintf(stderr, "%s\n", "Too few arguments.");
		exit(-1);
	}
	else if (argc == 1) // File redirection
	{
		if ((inputFile = fopen("temp.txt", "w+")) == NULL)
		{
			perror("Opening file 'temp.txt' failed");
		    exit(1);
		}
		if ((outputFile = fopen("out.asm", "w+")) == NULL)
		{
			perror("Opening file 'out.asm' failed");
		    exit(1);
		}

		FILE* readFromOut = stdin;
		int c;
		while((c = fgetc(readFromOut)) != EOF && !feof(readFromOut))
			fputc(c, inputFile);
		close(readFromOut);
		fseek(inputFile, 0, SEEK_SET);
	}
	else if (argc == 2) // Input file
	{
		char fileName[64];
		strcpy(fileName, argv[1]);
		printf("%s\n", argv[1]);
		strcat(fileName, ".fs17");
		printf("%s\n", fileName);
		if ((inputFile = fopen(fileName, "r")) == NULL)
		{
			perror("Opening file failed");
		    exit(1);
		}
		if ((outputFile = fopen("file.asm", "w+")) == NULL)
		{
			perror("Opening file 'file.asm' failed");
		    exit(1);
		}
	}
	else
	{
		fprintf(stderr, "%s\n", "Too many arguments.");
		exit(1);
	}
}

char *extractFileExtension(char *file)
{

	const char s[2] = ".";
	int count = 0;
	char *token = file;

	while(token != NULL)
		token = strtok(NULL, s);
	
	return token;
}
