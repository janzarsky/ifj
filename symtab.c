#include <stdio.h>

#include "symtab.h"

/*#include "stable.h"



void tableInit(tSymbolTable *T)
// funkce inicializuje tabulku symbolu
{

  if (T = malloc(sizeof(tTableItem)*TABLE_SIZE) == NULL) return -1;

  T->first = NULL;
}


unsigned int hash_function(const char *str, unsigned htab_size) {

    unsigned int h = 0;

    const unsigned char *p;

    for(p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599*h + *p;

    return h % htab_size;
}


void symbol_init(tTableItem **I, int idx){

    *I->bool = 0;
    *I->nextItem = NULL;
    *I->data->varType = -1;
    *I->data->varValue = -1;

    *I->key->str = '\0';
    *I->key->length = -1;
    *I->key->allocSize = -1;
    *I->key->type = -1;



}*/

void st_init(symtab_t *st) {
   st->size = 0; 
}

void st_insert(symtab_t *st, symtab_elem_t elem) {
    st->elements[st->size] = elem;
    st->size++;
}

void st_print_element(symtab_elem_t elem) {
    printf("DEBUG:   id: '%s'\n", elem.id);
    printf("DEBUG:   element type (0=variable, 1=function, 2=class): %d\n", (int) elem.elem_type);
    printf("DEBUG:   data type (0=int, 1=double, 2=string, 3=void): %d\n", (int) elem.data_type);

    switch (elem.data_type) {
        case ST_DATATYPE_INT:
            printf("DEBUG:   data value: %d\n", elem.value.ival);
            break;
        case ST_DATATYPE_DOUBLE:
            printf("DEBUG:   data value: %g\n", elem.value.dval);
            break;
        case ST_DATATYPE_STRING:
            printf("DEBUG:   data value: %s\n", elem.value.strval);
            break;
        case ST_DATATYPE_VOID:
            printf("DEBUG:   data value: void\n");
            break;
    }
}

void st_print(symtab_t *st) {
    printf("==========\n");
    printf("DEBUG: printing symtab\n");
    printf("DEBUG: number of elements: %lu\n", st->size);

    for (size_t i = 0; i < st->size; i++) {
        printf("DEBUG: printing element nr. %lu:\n", i);
        st_print_element(st->elements[i]);
    }

    printf("==========\n");
}
