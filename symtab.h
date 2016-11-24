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
    ST_DATATYPE_ERROR,
    ST_DATATYPE_VOID,
    ST_DATATYPE_INT,
    ST_DATATYPE_DOUBLE,
    ST_DATATYPE_STRING,
    ST_DATATYPE_BOOL
} st_datatype_t;

typedef union {
    int ival;
    double dval;
    char *strval;
} st_value_t;

typedef struct symtab_t symtab_t;

typedef struct symtab_elem_t {
    char *id;
    st_elemtype_t elem_type;
    st_datatype_t data_type;
    st_value_t value;
    bool declared;
    bool initialized;
    symtab_t *local_table;
    struct symtab_elem_t *nextElem;
} symtab_elem_t;

struct symtab_t{
    symtab_elem_t *elements[TABLE_SIZE];
    size_t size;
};

unsigned int hash_function(const char *str, unsigned htab_size);
void symbol_init(symtab_t **I, int idx);

int st_init(symtab_t **table);
symtab_elem_t *st_add(symtab_t *table, char *token);
symtab_elem_t *st_find (symtab_t *table, char *token);
void st_free(symtab_t *table);
void st_print_elem(symtab_elem_t *elem);
void st_print(symtab_t *table);
#endif
