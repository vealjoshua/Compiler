# Compiler
Making a compiler for my class.
Static Semantics: Local
Storage: Local
###### Warning: C novices proceed with extreme caution.

## Invocation

comp [file]
where file is an optional argument. If the file argument is not given the program will read data from the keyboard as a file device. If the argument is given, the program reads data file file.fs17. (note that file is any name as given and the extension is implicit).

## Output
If the argument file is given (and the input is file.fs17) then the program will produce output target file.asm. When reading from the keyboard, the output file should be out.asm.

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

-------------------
    Begin
      Input x ;
    End

--------------------
    Begin
     x : y ;
    End

--------------------
    Begin
      Check [ 0 == 0 ]
       Output 0 ;
    End

----------------------
    Begin
      Loop [ 0 == 0 ]
        Output 0 ;
    End

Now add variables e.g.

------------------------
    Var x .
    Begin
      Output 1 ;
    End

------------------------
    Var x , y .
    Begin
      Output 1 ;
    End
    
Now add 2 statements at a time.

Then add 3 statements at a time.

Then use some statements as blocks, and try without variables in the block and then with variables in the block. Etc.

-----------------------------

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
