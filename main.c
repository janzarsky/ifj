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

int main(int argc, char** argv) {
    if (argc != 2) {
        return ER_INTERN;
    }

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

    symtab_t *symtab;
    st_init(&symtab);

    st_add_builtin_functions(symtab);

    instr_list = malloc(sizeof(tListOfInstr));
    listInit(instr_list);

    setSourceFile(source);
    set_symtable(symtab);

    debug_printf("MAIN: parse code (first run)\n");

    int parse_result = program();

    debug_printf("******************************\n\nfirst run result: %d\n\n", parse_result);

#ifdef DEBUG
    printf("MAIN: symtab\n");
    st_print(symtab);
#endif

    if (parse_result != ER_OK)
       goto out;

    symtab_elem_t *temp = st_find(symtab, "Main.run");
    add_instr(IN_CALL, NULL, NULL, temp);
    set_function_beginning(&(temp->first_instr));

    debug_printf("\nMAIN: parse code (second run)\n");

    parse_result = program();
    
    debug_printf("******************************\n\nsecond run result: %d\n\n", parse_result);

#ifdef DEBUG
    printf("MAIN: symtab\n");
    st_print(symtab);

    printf("\nMAIN: generated instructions\n");
    print_instr_list();
#endif

    rewind(source);

    if (parse_result != ER_OK)
       goto out;

    debug_printf("\nMAIN: interpret code\n");

    int interpret_result = interpret(instr_list);

    debug_printf("******************************\n\nresult: %d\n", interpret_result);

#ifdef DEBUG
    printf("MAIN: symtab\n");
    st_print(symtab);
#endif

    return interpret_result;

out:
    fclose(source);

    return parse_result;
}
