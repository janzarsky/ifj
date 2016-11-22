#ifndef _SYMTAB_H
#define _SYMTAB_H

#include <stdbool.h>

#define TABLE_SIZE 1000

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
} symtab_elem_t;

typedef struct {
    symtab_elem_t *elements[TABLE_SIZE];
    size_t size;
} symtab_t;

unsigned int hash_function(const char *str, unsigned htab_size);
void symbol_init(symtab_t **I, int idx);
int tableInit(symtab_t **T);
void symbol_add(char *token, symtab_t **tabulka);
symtab_elem_t *symbol_find (char *token, symtab_t *tabulka);
void symbol_actualize (symtab_t *T, char *ident);
void tableFree(symtab_t *T);
void st_insert(symtab_t *st, symtab_elem_t elem);

void st_print(symtab_t *st);
#endif
