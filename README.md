# Compiler
Making a compiler for my class.
###### Warning: C novices proceed with extreme caution.

## Instructor Specifications
Implement the parser in a separate file (parser.c and parser.h) including the initial auxilary parser() function and all nonterminal functions. Call the parser function from main.
The parser function generates error or returns the parse tree to main.
In testTree.c (and testTree.h) implement a printing function using preorder traversal with indentations as before for testing purposes (2 spaces per level, print the node's label and any tokens from the node, then children left to right; one node per line).
Call the printing function from main immediately after calling the parser and returning the tree.
The printing function call must be later removed.

## CFG
\<program\>   ->   \<vars\> \<block\>

\<block\>     ->  Begin \<vars\> \<stats\> End

\<vars\>      ->  ɛ | Var Identifier \<mvars\> 

\<mvars\>     ->  . | , Identifier \<mvars\>

\<expr\>      ->  \<M\> + \<expr\> | \<M\> - \<expr\> | \<M\>

\<M\>         ->  \<F\> % \<M\> | \<F\> * \<M\> | \<F\>

\<F\>         ->  ( \<F\> ) | \<R\>

\<R\>         ->  [ \<expr\> ] | Identifier | Number

\<stats\>     ->  \<stat\>  \<mStat\>

\<mStat\>     ->  ɛ |  \<stat\>  \<mStat\>

\<stat\>      ->  \<in\> | \<out\> | \<block\> | \<if\> | \<loop\> | \<assign\>

\<in\>        ->  Input Identifier  ; 

<out>         ->  Output \<expr\> ;

\<if\>        ->  Check [ \<expr\> \<RO\> \<expr\> ] \<stat\>

\<loop\>      ->  Loop [ \<expr\> \<RO\> \<expr\> ] \<stat\>

\<assign\>    ->  Identifier : \<expr\> ;

\<RO\>          ->  < | <= | >  | >= | ==  |  !=

## Test files for good programs
Create files using the algorithm to generate programs from the grammar, starting with simplest programs one different statement at a time and then building sequences of statements and nested statements. You may skip comments but then test comment in some files.

Examples. Start with simplest programs and no variables and no comments.


    Begin
      Output 1 ;
    End

------------------
    Begin
      Input x ;
    End

-------------------
    Begin
      x : y ;
    End

--------------------
    Begin
      Check [ 0 == 0 ]
        Output 0 ; 
    End

---------------------
    Begin
      Loop [ 0 == 0 ]
        Output 0 ;
    End

Now add variables e.g.

-----------------------
    Var x .
    Begin
      Output 1 ;
    End

-----------------------
    Var x , y .
    Begin
      Output 1 ;
    End

Now add 2 statements at a time.

Then add 3 statements at a time.

Then use some statements as blocks, and try without variables in the block and then with variables in the block. Etc.

----------------------------

    Var x , y .
    Begin
      Input x ;
      Output 2 ;
      Begin
        Var x1 , x2 .
        x : 2 + 3 % y ;
      End
      Loop [ x + 2 <= y + [ 2 - 3] + ( y )  ]
        Begin
          Loop [ x == ( [ x ] ) ]
            Begin
              Var x2 .
              Output 1 ;
            End
        End
    End
