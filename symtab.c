#include <stdio.h>
#include <string.h>

#include "symtab.h"

/*
#include "stable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void tableInit(SymbolTable *T)
// funkce inicializuje tabulku symbolu
{

  if (T = (struct tTableItem *)malloc(sizeof(tTableItem)*TABLE_SIZE) == NULL) return -1;

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        symbol_init(T, i);
    }

}


unsigned int hash_function(const char *str, unsigned htab_size) {

    unsigned int h = 0;

    const unsigned char *p;

    for(p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599*h + *p;

    return h % htab_size;
}


void symbol_init(SymbolTable *I, int idx){

    I->table_items[idx]->free = 0;
    I->table_items[idx]->nextItem = NULL;
    I->table_items[idx]->data.varType = -1;
    //I->data.varValue = -1;

    I->table_items[idx]->token.str = '\0';
    I->table_items[idx]->token.length = -1;
    I->table_items[idx]->token.allocSize = -1;
   // I->table_items[idx]->token.type = -1;

}

void symbol_add(string token, SymbolTable *tabulka){

char *tokenchar = token.str;


unsigned int klic = hash_function(tokenchar, TABLE_SIZE);

if (tabulka->table_items[klic]->free) {

tabulka->table_items[klic]->token = token;
tabulka->table_items[klic]->free = false;

}

else {
    struct tableItem *synon = malloc((struct tableItem *)sizeof(struct tableItem));
    if (synon == NULL)
        return;
    synon->token = token;
    synon->free = false;
    synon->nextItem = NULL;
    tabulka->table_items[klic]->nextItem = synon;
}
}

void symbol_find (string token, SymbolTable *tabulka){

char *tokenchar = token.str;

unsigned int klic = hash_function(tokenchar, TABLE_SIZE);

while (tabulka->table_items[klic]->nextItem != NULL){

    if (tabulka->table_items[klic]->token.str == tokenchar) return; //SYNTAX_ERROR;

}

}

void tableFree(SymbolTable *T)
// funkce dealokuje tabulku symbolu
{
  tTableItem *ptr;

  for (int i = 0; i <TABLE_SIZE; i++)
  {
      while (T->table_items[i]->nextItem != NULL)
      {
        ptr = T->table_items[i]->nextItem;
        T->table_items[i]->nextItem = T->table_items[i]->nextItem->nextItem;
        // uvolnime klic
        strFree(&ptr->token);
        // nakonec uvolnime celou polozku
        free(ptr);
     }
  }
}
*/

void st_init(symtab_t *st) {
   st->size = 0; 
}

void st_insert(symtab_t *st, symtab_elem_t elem) {
    st->elements[st->size] = elem;
    st->size++;
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
        st_print_element(&(st->elements[i]));
    }

    printf("==========\n");
}

symtab_elem_t *st_find(symtab_t *st, char *id) {
   for (size_t i = 0; i < st->size; i++) {
       if (strcmp(st->elements[i].id, id) == 0) {
           return &(st->elements[i]);
       }
   }

   return NULL;
}
