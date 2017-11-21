<<<<<<< HEAD
Option 3. FSA table + driver (100)
The table data is in scanner.c. The driver uses lookahead.
The program does not check the redirected input filename, though there is a function to extract the extension. 

You can't get the filename exactly as input; the shell will handle all that redirection stuff without telling you.

In the case of a direct < file redirection, you can retrieve a filepath associated with stdin by using  fstat to get an inode number for it then walking the file hierarchy similarly to find / -inum to get a path that matches it. (There might be more than one such filepath due to links.)

But you shouldn't ever need to do this. As others have said, if you need to know filenames you should be taking filenames as arguments.

https://stackoverflow.com/questions/1734243/in-c-how-do-i-print-filename-of-file-that-is-redirected-as-input-in-shell

Compiler gives a warning:

testScanner.c: In function âtestScannerâ:
testScanner.c:20:24: warning: initialization makes pointer from integer without a cast [enabled by default]
   tokenType** tokens = scanner(str, lineNumber++, &numOfTokens);

scanner(str, lineNumber++, &numOfTokens) returns a tokenType\*\* which points to the address of the first tokenType pointer. I assume tokens is pointing to the same address. Addresses are of type integer, maybe that has something to do with it.
=======
# Compiler
Making a compiler for my class. Warning: C novices proceed with extreme caution.
>>>>>>> db0f5657d363541bddc270765777d46469b88cf8
