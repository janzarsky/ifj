#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symtab.h"
#include "string.h"



#define TABLE_SIZE 1000
unsigned int hash_function(const char *str, unsigned htab_size) {
    unsigned int h = 0;
    const unsigned char *p;
    for(p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
}
void symbol_init(symtab_t **I, int idx){
    (*I)->elements[idx].free = true;
    (*I)->elements[idx].nextElem = NULL;
    (*I)->elements[idx].data_type = -1;
    (*I)->elements[idx].elem_type = -1;
    (*I)->elements[idx].value.dval = 0;
    (*I)->elements[idx].value.ival = 0;
    (*I)->elements[idx].value.strval = '\0';
    (*I)->elements[idx].id = '\0';
    (*I)->elements[idx].token.str = '\0';
    (*I)->elements[idx].token.allocSize = 0;
    (*I)->elements[idx].token.length = 0;
}
int tableInit(symtab_t **T){
  *T = (symtab_t *)malloc(sizeof(symtab_elem_t)*TABLE_SIZE);
  if (T == NULL) return - 1;
  //if (T = (struct symtab_t *)malloc(sizeof(symtab_elem_t)*TABLE_SIZE) == NULL) return -1;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        symbol_init(T, i);
    }
  return 0;
}
void symbol_add(string *token, symtab_t **tabulka){
char *tokenchar = token->str;
unsigned int klic = hash_function(tokenchar, TABLE_SIZE);
printf("%u\n",klic);
if ((*tabulka)->elements[klic].free == true) {
(*tabulka)->elements[klic].token.allocSize = token->allocSize;
(*tabulka)->elements[klic].token.length = token->length;
(*tabulka)->elements[klic].token.str = token->str;
(*tabulka)->elements[klic].free = false;
// DODELAT VALUE, DATA TYPE A ELEM TYPE!!!
}
else {
printf("jsem v else \n");
    symtab_elem_t *synon = (symtab_elem_t *)malloc(sizeof(symtab_elem_t));
    if (synon == NULL) return;
    synon->token = *token;
    synon->free = false;
    synon->nextElem = NULL;
    (*tabulka)->elements[klic].nextElem = synon;
}
}
symtab_elem_t *symbol_find (string *token, symtab_t *tabulka){
    char *tokenchar = token->str;
    unsigned int klic = hash_function(tokenchar, TABLE_SIZE);
    if (tabulka->elements[klic].nextElem == NULL){
        if (tabulka->elements[klic].token.str != tokenchar) return NULL;//SYNTAX_ERROR;
        else{
       //symtab_elem_t *symbol = (symtab_elem_t *)malloc(sizeof(symtab_elem_t));
       //*symbol = tabulka->elements[klic];
            return &(tabulka->elements[klic]);}
    }
    else
    {
        symtab_elem_t *ptr = tabulka->elements[klic].nextElem;
        symtab_elem_t *found = NULL;
        while (ptr != NULL)
        {
            if (ptr->token.str == tokenchar)
                found = ptr;
            ptr = ptr->nextElem;
        }
        return found;
    }
}
void symbol_actualize (symtab_t *T, string ident)
{
    symtab_elem_t *ptr;
    ptr = symbol_find(&ident, T);
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
      while (T->elements[i].nextElem != NULL)
      {
        ptr = T->elements[i].nextElem;
        T->elements[i].nextElem = T->elements[i].nextElem->nextElem;
        // uvolnime klic
        strFree(&ptr->token);
        // nakonec uvolnime celou polozku
        free(ptr);
     }
  }
  free(T);
}


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

/*
int main()
{

   symtab_t *tabulka = NULL;

   // INICIALIZACE TABULKY

   int inicializace = tableInit(&tabulka);

   if (inicializace == -1 ) {printf("tableInit failed\n"); return -1;}

   bool volno =  tabulka->elements[496].free;

   if (volno == true)
        printf("volno\n");

   if (tabulka->elements[496].free == true && tabulka->elements[496].nextElem == NULL && tabulka->elements[496].data_type == -1 && tabulka->elements[496].elem_type == -1 && tabulka->elements[496].value.dval == 0 && tabulka->elements[496].value.ival == 0 && tabulka->elements[496].value.strval == '\0' && tabulka->elements[496].id == '\0' && tabulka->elements[496].token.str == '\0' && tabulka->elements[496].token.allocSize == 0 && tabulka->elements[496].token.length == 0)
        printf("init OK\n");
   printf("PRIDAVANI SYMBOLU\n");


   string *novySymbol = (string *)malloc(sizeof(string));
    printf("malloc_OK\n");
   novySymbol->allocSize = 8;
   novySymbol->length = 5;
   novySymbol->str = "Ahoj";

   symbol_add(novySymbol, &tabulka);
   printf("%s\n",tabulka->elements[498].token.str);
   volno =  tabulka->elements[498].free;

   if (volno == false)
        printf("!volno\n");
    else
        printf("volno");

   novySymbol->allocSize = 8;
   novySymbol->length = 3;
   novySymbol->str = "Uz";
   unsigned int klic = hash_function(novySymbol->str, TABLE_SIZE);
   symbol_add(novySymbol, &tabulka);
   printf("%s\n",tabulka->elements[klic].token.str);
   volno =  tabulka->elements[klic].free;

   novySymbol->allocSize = 8;
   novySymbol->length = 5;
   novySymbol->str = "Ahoj";
   klic = hash_function(novySymbol->str, TABLE_SIZE);
   symbol_add(novySymbol, &tabulka);
   printf("%s\n",tabulka->elements[klic].nextElem->token.str);
   volno =  tabulka->elements[klic].nextElem->free;
   if (volno == false)
        printf("!volno\n");
    else
        printf("volno");

    symtab_elem_t *find = symbol_find(novySymbol, tabulka);
    printf("%s\n",find->token.str);
    if (find == tabulka->elements[498].nextElem)
        printf("OK\n");

    tableFree(tabulka);
        printf("%p\n", tabulka);
   }

*/
