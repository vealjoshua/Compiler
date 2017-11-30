#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "extern.h"
#include "parser.h"
#include "testTree.h"
#include "tree.h"

extern FILE* fp;

FILE* processArgs(int argc, char* argv[], char** fn);
char *extractFileExtension(char *file);

int main(int argc, char* argv[])
{
	char* fileName = NULL;
	fp = processArgs(argc, argv, &fileName);
	node* root = parser();
	fclose(fp);
	if (argc == 2)
		remove("temp.txt");
	testTree(root, 0);
	deleteTree(root);
	return 0;
}

FILE* processArgs(int argc, char* argv[], char** fn)
{
	FILE* fp;
	if (argc < 2)
	{
		fprintf(stderr, "%s\n", "Too few arguments.");
		exit(-1);
	}
	else if (argc == 2) // File redirection
	{
		fp = fopen("temp.txt", "w+");
		if (fp == NULL)
		{
			perror("Opening file 'temp.txt' failed");
		    exit(1);
		}
		FILE* fp2 = stdin;

		int c;
		while((c = fgetc(fp2)) != EOF && !feof(fp))
			fputc(c, fp);
		fseek(fp, 0, SEEK_SET);

		close(fp2);
		*fn = "out.asm";
	}
	else if (argc == 3) // Input file
	{
		if ((fp = fopen(argv[2], "r")) == NULL)
		{
			perror("Opening file failed");
		    exit(1);
		}
		*fn = argv[2];
	}
	else
	{
		fprintf(stderr, "%s\n", "Too many arguments.");
		exit(1);
	}

	return fp;
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
