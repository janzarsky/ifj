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
extern symtab_t *local_tabulka;

int main(int argc, char** argv) {
    if (argc != 2) {
        return ER_INTERN;
    }

    // open file
    FILE *source;

    if ((source = fopen(argv[1], "r")) == NULL) {
        return ER_INTERN;
    }
    
#ifdef DEBUG
    printf("MAIN: source code:\n");

    int c;

    while ((c = getc(source)) != EOF)
        putchar(c);
    
    printf("*****\n");
    
    fclose(source);

    if ((source = fopen(argv[1], "r")) == NULL) {
        return ER_INTERN;
    }
#endif

    // initialize table of symbols
    symtab_t *symtab;
    st_init(&symtab);

    st_add_builtin_functions(symtab);

    // initialize list of instructions
    instr_list = malloc(sizeof(tListOfInstr));
    listInit(instr_list);

    setSourceFile(source);
    set_symtable(symtab);

    // run parser (first run)
    debug_printf("MAIN: parse code (first run)\n");

    int result = program();

    debug_printf("******************************\n\nfirst run result: %d\n\n", result);

#ifdef DEBUG
    printf("MAIN: symtab\n");
    st_print(symtab);
#endif

    if (result != ER_OK)
       goto out;

    local_tabulka = NULL;

    symtab_elem_t *temp = st_find(symtab, "Main.run");

    if (temp == NULL) {
        result = ER_SYNTAX;
        goto out;
    }

    add_instr(IN_CALL, NULL, NULL, (void *) temp);
    set_function_beginning(&(temp->first_instr));

    // run parser (second run)
    debug_printf("\nMAIN: parse code (second run)\n");

    rewind(source);

    result = program();
    
    debug_printf("******************************\n\nsecond run result: %d\n\n", result);

#ifdef DEBUG
    printf("MAIN: symtab\n");
    st_print(symtab);

    printf("\nMAIN: generated instructions\n");
    print_instr_list();
#endif

    if (result != ER_OK)
       goto out;

    // run interpret
    debug_printf("\nMAIN: interpret code\n");

    result = interpret(instr_list);

    debug_printf("******************************\n\nresult: %d\n", result);

#ifdef DEBUG
    printf("MAIN: symtab\n");
    st_print(symtab);
#endif

    debug_printf("MAIN: freeing symtab\n");
    st_free(symtab);
    debug_printf("MAIN: freeing instr_list\n");
    listFree(instr_list);
    debug_printf("MAIN: closing file\n");
    fclose(source);

    return result;

out:
    st_free(symtab);
    fclose(source);

    return result;
}
