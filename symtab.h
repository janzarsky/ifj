#ifndef _SYMTAB_H
#define _SYMTAB_H
/*
#define T_INT    1
#define T_DOUBLE 2
#define T_STRING 3
#include <stdbool.h>

#define TABLE_SIZE 1000

typedef struct string_struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string;

typedef struct
{
  int varType;  // typ dane promenne, v nasem pripade vzdy int
  union varValue; // pro ruzne typy nutnost resit pomoci unie
} tData;


union varValue {
   int i;
   float f;
   char *s;
};

typedef struct tableItem
{
  string token;                  // klic, podle ktereho se bude vyhledavat = nazev identifikatoru
  tData data;                  // data, ktera jsou ke klici pridruzena
  struct tableItem *nextItem;  // ukazatel na dalsi prvek tabulky symbol
  bool free;
} tTableItem;

typedef struct
{
  struct tableItem *table_items[TABLE_SIZE];
} SymbolTable;

*/
typedef enum {
    ST_ELEMTYPE_VAR,
    ST_ELEMTYPE_FUN,
    ST_ELEMTYPE_CLASS
} st_elemtype_t;

typedef enum {
    ST_DATATYPE_INT,
    ST_DATATYPE_DOUBLE,
    ST_DATATYPE_STRING,
    ST_DATATYPE_VOID
} st_datatype_t;

typedef union {
    int ival;
    double dval;
    char *strval;
} st_value_t;

typedef struct {
    char *id;
    st_elemtype_t elem_type;
    st_datatype_t data_type;
    st_value_t value;
} symtab_elem_t;

typedef struct {
    symtab_elem_t elements[1000];
    size_t size;
} symtab_t;

void st_init(symtab_t *st);
void st_insert(symtab_t *st, symtab_elem_t elem);
void st_print(symtab_t *st);

#endif
