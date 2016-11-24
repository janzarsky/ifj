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

#include <stdio.h>
#include <stdlib.h>

#include "instrlist.h"
#include "parser_test.h"
#include "scanner.h"
#include "symtab.h"

//DEBUG
void symtab_test()
{

   symtab_t *tabulka = NULL;

   // INICIALIZACE TABULKY

   int inicializace = st_init(&tabulka);

   if (inicializace == -1 ) {
       printf("tableInit failed\n");
       return;
   }

   bool volno = tabulka->elements[496] == NULL;

   if (volno == true)
        printf("volno\n");

   printf("PRIDAVANI SYMBOLU\n");

   char *novySymbol = "Ahoj";

   st_add(tabulka, novySymbol);
   printf("%s\n",tabulka->elements[498]->id);
   volno =  tabulka->elements[498] == NULL;

   if (volno == false)
        printf("!volno\n");
    else
        printf("volno");

   novySymbol = "Uz";
   unsigned int klic = hash_function(novySymbol, TABLE_SIZE);
   st_add(tabulka, novySymbol);
   printf("%s\n",tabulka->elements[klic]->id);
   volno =  tabulka->elements[klic] == NULL;

   novySymbol = "Ahoj";
   klic = hash_function(novySymbol, TABLE_SIZE);
   st_add(tabulka, novySymbol);
   printf("%s\n",tabulka->elements[klic]->nextElem->id);
   volno =  tabulka->elements[klic]->nextElem == NULL;
   if (volno == false)
        printf("!volno\n");
    else
        printf("volno");

    symtab_elem_t *find = st_find(tabulka, novySymbol);
    printf("%s\n",find->id);
    if (find == tabulka->elements[498]->nextElem)
        printf("OK\n");

    st_free(tabulka);
    printf("%p\n", (void *) tabulka);
}

int main(int argc, char** argv) {
    //symtab_test();

    if (argc != 2) {
        return 99;
    }

    FILE *source;

    if ((source = fopen(argv[1], "r")) == NULL) {
        return 99;
    }

    // initialize table of symbols
    symtab_t *symtab;
    st_init(&symtab);

    symtab_t *symtab_local;
    st_init(&symtab_local);

    tListOfInstr *ilist = malloc(sizeof(tListOfInstr));
    listInit(ilist);

    // table of symbols == NULL (no table implemented yet)
    setSourceFile(source);
    set_symtable(symtab);
    set_symtable_local(symtab_local);

    int parse_result = program();

    // check error code
    printf("result: %d\n", parse_result);

    // interpret code

    // free table of symbols
    // free instruction list
    fclose(source);

    return 0;
}
