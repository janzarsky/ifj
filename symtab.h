#ifndef _SYMTAB_H
#define _SYMTAB_H

#include <stdbool.h>
#include "string.h"
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
typedef struct symtab_elem_t{
    char *id;
    st_elemtype_t elem_type;
    st_datatype_t data_type;
    st_value_t value;
    struct symtab_elem_t *nextElem;
    bool free;
    string token;
} symtab_elem_t;
typedef struct {
    symtab_elem_t elements[1000];
    size_t size;
} symtab_t;
unsigned int hash_function(const char *str, unsigned htab_size);
void symbol_init(symtab_t **I, int idx);
int tableInit(symtab_t **T);
void symbol_add(string *token, symtab_t **tabulka);
symtab_elem_t *symbol_find (string *token, symtab_t *tabulka);
void symbol_actualize (symtab_t *T, string ident);
void tableFree(symtab_t *T);
#endif
