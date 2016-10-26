#ifndef _SYMTAB_H
#define _SYMTAB_H

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
