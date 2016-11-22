#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "symtab.h"

unsigned int hash_function(const char *str, unsigned htab_size) {
    unsigned int h = 0;
    const unsigned char *p;
    for(p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
}

int tableInit(symtab_t **T){
    *T = (symtab_t *) malloc(sizeof(symtab_elem_t)*TABLE_SIZE);

    if (T == NULL)
        return - 1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        (*T)->elements[i] = NULL;
    }

    return 0;
}

void symbol_add(char *token, symtab_t **tabulka) {
    unsigned int klic = hash_function(token, TABLE_SIZE);
    printf("%u\n",klic);

    symtab_elem_t *synon = (symtab_elem_t *) malloc(sizeof(symtab_elem_t));

    if (synon == NULL)
        return;

    synon->id = token;

    if ((*tabulka)->elements[klic] != NULL) {
        synon->nextElem = (*tabulka)->elements[klic]->nextElem;
        (*tabulka)->elements[klic]->nextElem = synon;
    }
    else {
        synon->nextElem = NULL;
        (*tabulka)->elements[klic] = synon;
    }

}

symtab_elem_t *symbol_find (char *token, symtab_t *tabulka){
    unsigned int klic = hash_function(token, TABLE_SIZE);

    if (tabulka->elements[klic]->nextElem == NULL) {
        if (tabulka->elements[klic]->id != token)
            return NULL;//SYNTAX_ERROR;
        else
            return tabulka->elements[klic];
    }
    else {
        symtab_elem_t *ptr = tabulka->elements[klic]->nextElem;
        symtab_elem_t *found = NULL;

        while (ptr != NULL)
        {
            if (ptr->id == token)
                found = ptr;

            ptr = ptr->nextElem;
        }
        return found;
    }
}

void symbol_actualize (symtab_t *T, char *ident)
{
    symtab_elem_t *ptr;
    ptr = symbol_find(ident, T);
    if (ptr == NULL)
        //error;
        return;
    // DODELAT VALUE, DATA TYPE A ELEM TYPE!!!
}

void tableFree(symtab_t *T)
// funkce dealokuje tabulku symbolu
{
    symtab_elem_t *ptr;
    for (int i = 0; i <TABLE_SIZE; i++)
    {
        if (T->elements[i] != NULL) {
            while (T->elements[i]->nextElem != NULL) {
                ptr = T->elements[i]->nextElem;
                T->elements[i]->nextElem = T->elements[i]->nextElem->nextElem;
                free(ptr);
            }

            free(T->elements[i]);
        }
    }
    free(T);
}


void st_init(symtab_t *st) {
   st->size = 0; 
}

void st_print_element(symtab_elem_t *elem) {
    printf("DEBUG:   id: '%s'\n", elem->id);
    printf("DEBUG:   element type (0=variable, 1=function, 2=class): %d\n", (int) elem->elem_type);
    printf("DEBUG:   data type (0=int, 1=double, 2=string, 3=void): %d\n", (int) elem->data_type);

    switch (elem->data_type) {
        case ST_DATATYPE_INT:
            printf("DEBUG:   data value: %d\n", elem->value.ival);
            break;
        case ST_DATATYPE_DOUBLE:
            printf("DEBUG:   data value: %g\n", elem->value.dval);
            break;
        case ST_DATATYPE_STRING:
            printf("DEBUG:   data value: %s\n", elem->value.strval);
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
