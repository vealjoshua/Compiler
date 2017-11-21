#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testScanner.h"

// Scanner implementation

char *extractFileExtension(char *file);

int main(int argc, char* argv[])
{
	FILE* fp;

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
		fgets(str, SIZE, fp);
		fclose(fp);

		// Write to file, create one if it doesn't exist, close file, open file for reading because it wouldn't work if done all at once
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

	testScanner(fp);
	fclose(fp);
	if (argc == 2)
		remove("temp.txt");

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
