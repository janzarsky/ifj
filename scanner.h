#include "string.h"

#ifndef _SCANNER_H
#define _SCANNER_H

//identifikator
#define ID       0

//klicova slova
#define BOOLEAN    10
#define BREAK      11
#define CLASS      12
#define CONTINUE   13
#define DO         14
#define DOUBLE     15
#define ELSE       16
#define FALSE      17
#define FOR        18
#define IF         19
#define INT        20
#define RETURN     21
#define STRING     22
#define STATIC     23
#define TRUE       24
#define VOID       25
#define WHILE      26

// povinne funkce

#define MAIN 27
#define RUN  28

//operatory
#define PLUS     30
#define MINUS    31
#define MUL      32
#define DIV      33
#define LESS     34 // <
#define GREAT    35 // >
#define LESS_EQ  36 // <=
#define GREAT_EQ 37 // >=
#define EQUAL    38 // ==
#define N_EQUAL  39 // !=
#define ASSIGN   40 // =

//znaky
#define LEFT_VINCULUM  41 // '{'   - pouzivano u trid a funkci
#define RIGHT_VINCULUM 42 // '}'   - pouzivano u trid a funkci
#define LEFT_BRACKET   43 // '('   - pouzivano u funkci a cyklu
#define RIGHT_BRACKET  44 // ')'   - pouzivano u funkci a cyklu
#define SEMICOLON      45 // ';'
#define COMMA          46 // ,

//literaly

#define STRING_LITERAL 50
#define INT_LITERAL    51
#define DOUBLE_LITERAL 52

//specialni znaky
#define END_OF_FILE    60

// rozsireni projektu

#define INC 70 // operator inkrementace ++
#define DEC 71 // operator dekrementace --

//hlavicka funkce simulujici lexikalni analyzator
void setSourceFile(FILE *f);
int get_next_token(char **buffer);
void return_token(int token, char *buffer);

#endif
