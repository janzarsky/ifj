/**
 * Implementace interpretu imperativniho jazyka IFJ16
 * 
 * xzarsk03   Jan Zarsky
 * xvlcek23   David Vlcek
 * xpelan04   Pelantova Lucie
 * xmrlik00   Vit Mrlik
 * xpapla00   Andrei Paplauski
 *
 */

#ifndef _INSTRLIST_H
#define _INSTRLIST_H

// TOKENY

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
#define EQUAL    38
#define N_EQUAL  39
#define ASSIGN   40 // ==

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

// je garantovane, ze vsechny tokeny jsou mensi nez TOKEN_MAX
#define TOKEN_MAX      61


// INSTRUKCE

#define IN_ADD          0   // operace pro int nad dvema prvky na zasobniku
#define IN_SUB          1
#define IN_MUL          2
#define IN_DIV          3
#define IN_F_ADD        4   // operace pro double
#define IN_F_SUB        5
#define IN_F_MUL        6
#define IN_F_DIV        7
#define IN_TAB_PUSH     8   // vloz na zasobnik
#define IN_VAL_PUSH     9
#define IN_CONV         10   // preved vrchol zasobniku z int na double
#define IN_SWAP         11  // vymen horni dva prvky zasobniku
#define IN_CONCAT       12  // spoj horni dva retezce na zasobniku
#define IN_LESS         13  // porovna horni dva inty
#define IN_GREAT        14
#define IN_LESS_EQ      15
#define IN_GREAT_EQ     16
#define IN_EQ           17
#define IN_N_EQ         18
#define IN_F_LESS       19  // porovna horni dva doubly
#define IN_F_GREAT      20
#define IN_F_LESS_EQ    21
#define IN_F_GREAT_EQ   22
#define IN_F_EQ         23
#define IN_F_N_EQ       24

#define IN_GOTO         25
#define IN_IFNGOTO      26
#define IN_JMP_WHILE    27

#define IN_LABEL        32
#define IN_MOVSTACK     33
#define IN_RETURN       34
#define IN_CALL         35
#define IN_CONV_SYMBOL  36
#define IN_MOV          37

#define IN_INT_TO_STR   38
#define IN_DBL_TO_STR   39

// pomocne defines pro zjednoduseni kodu
// instrukce pro double se daji ziskat jako (instrukce pro int) + F_***_OFFSET
#define F_ARITH_OFFSET  (IN_F_ADD - IN_ADD)
#define F_REL_OFFSET    (IN_F_LESS - IN_LESS)


typedef struct {
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem {
  tInstr instruction;
  struct listItem *nextItem;
} tListItem;


typedef struct {
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void listInit(tListOfInstr *L); // funkce inicializuje seznam instrukci
void listFree(tListOfInstr *L); // funkce dealokuje seznam instrukci
void listInsertLast(tListOfInstr *L, tInstr I); // vlozi novou instruci na konec seznamu
void listFirst(tListOfInstr *L); // zaktivuje prvni instrukci
void listNext(tListOfInstr *L); // aktivni instrukci se stane nasledujici instrukce
tInstr *listGetData(tListOfInstr *L); // vrati aktivni instrukci
void listGoto(tListOfInstr *L, tListItem *gotoInstr); // nastavime aktivni instrukci podle zadaneho ukazatele
void *listGetPointerLast(tListOfInstr *L); // vrati ukazatel na posledni instrukci

void add_instr(int type, void * ptr1, void * ptr2, void * ptr3);
void set_function_beginning(tListItem **item);
void print_instr(tInstr *instr);
void print_instr_list();

#endif
