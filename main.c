#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "extern.h"
#include "parser.h"
#include "testTree.h"
#include "tree.h"

extern FILE* fp;

char *extractFileExtension(char *file);

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("Too few arguments.\n");
		exit(1);
	}
	else if (argc == 2)
	{
		// Read from stdin
		fp = stdin;

		size_t SIZE = 1024;
		char str[SIZE];

		printf("Please enter a string to return tokens.\n");
		fread(str, 1, sizeof(str), fp);
		fclose(fp);

		if ((fp = fopen("temp.txt", "w+")) == NULL)
		{
		    perror("Opening file failed");
		    exit(1);
		}

		fprintf(fp, "%s", str);
		fseek(fp, 0, SEEK_SET);
	}
	else if (argc == 3)
	{
		// read from file
		if ((fp = fopen(argv[2], "r")) == NULL)
		{
		    perror("Opening file failed");
		    exit(1);
		}
	}
	else {
		printf("Too many arguments.\n");
		exit(1);
	}

	node* root = parser();
	fclose(fp);
	if (argc == 2)
		remove("temp.txt");
	testTree(root, 0);
	deleteTree(root);
	// free(root);
	return 0;
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
