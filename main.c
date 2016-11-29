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
#include "frames.h"
#include "interpret.h"
#include "error_codes.h"
#include "debug.h"

extern tListOfInstr *instr_list;

#ifdef DEBUG
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

void frames_test() {
    symtab_t *symtab;
    st_init(&symtab);

    symtab_elem_t *func = st_add(symtab, "my_func");
    func->data_type = ST_DATATYPE_INT;
    func->elem_type = ST_ELEMTYPE_FUN;

    st_init(&(func->local_table));

    symtab_elem_t *func_param = st_add(func->local_table, "a");
    func_param->data_type = ST_DATATYPE_INT;
    func_param->elem_type = ST_ELEMTYPE_PARAM;

    symtab_elem_t *func_param2 = st_add(func->local_table, "b");
    func_param2->data_type = ST_DATATYPE_INT;
    func_param2->elem_type = ST_ELEMTYPE_PARAM;

    symtab_elem_t *func_param3 = st_add(func->local_table, "c");
    func_param3->data_type = ST_DATATYPE_INT;
    func_param3->elem_type = ST_ELEMTYPE_PARAM;

    func->first_param = func_param3;
    func_param3->next_param = func_param2;
    func_param2->next_param = func_param;
    func_param->next_param = NULL;

    printf("DEBUG: symtab\n");
    st_print(symtab);

    instr_list = malloc(sizeof(tListOfInstr));
    listInit(instr_list);

    inter_stack stack;
    stack.top = NULL;

    // fill stack
    inter_stack_item *temp;
    temp = malloc(sizeof(inter_stack_item));
    if (temp == NULL) return;
    temp->next = stack.top;
    temp->value.vval = (void *) 5;
    stack.top = temp;
    temp = malloc(sizeof(inter_stack_item));
    if (temp == NULL) return;
    temp->next = stack.top;
    temp->value.vval = (void *) 42;
    stack.top = temp;
    temp = malloc(sizeof(inter_stack_item));
    if (temp == NULL) return;
    temp->next = stack.top;
    temp->value.vval = (void *) 100;
    stack.top = temp;

    call_instr(instr_list, &stack, func);

    printf("DEBUG: instrlist\n");
    print_instr_list();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        return ER_INTERN;
    }

    FILE *source;

    if ((source = fopen(argv[1], "r")) == NULL) {
        return ER_INTERN;
    }

    printf("MAIN: source code:\n");

    int c;

    while ((c = getc(source)) != EOF)
        putchar(c);
    
    printf("*****\n");
    
    fclose(source);

    if ((source = fopen(argv[1], "r")) == NULL) {
        return 99;
    }

    // initialize table of symbols
    symtab_t *symtab;
    st_init(&symtab);

    instr_list = malloc(sizeof(tListOfInstr));
    listInit(instr_list);

    // table of symbols == NULL (no table implemented yet)
    setSourceFile(source);
    set_symtable(symtab);

    printf("MAIN: parse code\n");

    int parse_result = program();
    printf("******************************\n\nresult: %d\n", parse_result);

    //FIXME remove later
    symtab_elem_t *temp = st_find(symtab, "run");
    add_instr(IN_CALL, NULL, NULL, temp);
    set_function_beginning(&(temp->first_instr));

    printf("MAIN: symtab\n");
    st_print(symtab);

    rewind(source);
    parse_result = program();
    printf("******************************\n\nresult: %d\n", parse_result);

    printf("MAIN: symtab\n");
    st_print(symtab);

    //FIXME remove later
    //add_instr(IN_HALT, NULL, NULL, NULL);
    //printf("\n");

    printf("\nMAIN: generated instructions\n");
    print_instr_list();
    
    if (parse_result != ER_OK)
        return 1;

    printf("\nMAIN: interpret code\n");

    int interpret_result = interpret(symtab, instr_list);
    printf("******************************\n\nresult: %d\n", interpret_result);

    printf("MAIN: symtab\n");
    st_print(symtab);

    // free table of symbols
    // free instruction list
    fclose(source);

    return 0;
}
#else
int main(int argc, char** argv) {
    if (argc != 2) {
        return ER_INTERN;
    }

    FILE *source;

    if ((source = fopen(argv[1], "r")) == NULL) {
        return ER_INTERN;
    }
    
    symtab_t *symtab;
    st_init(&symtab);

    instr_list = malloc(sizeof(tListOfInstr));
    listInit(instr_list);

    setSourceFile(source);
    set_symtable(symtab);

    int parse_result = program();

    if (parse_result != ER_OK)
        goto out;

    rewind(source);

    parse_result = program();

out:
    fclose(source);

    return parse_result;
}
#endif
