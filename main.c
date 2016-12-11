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
#include "parser.h"
#include "scanner.h"
#include "symtab.h"
#include "frames.h"
#include "interpret.h"
#include "error_codes.h"

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
    int result = program();

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
    rewind(source);

    result = program();

    if (result != ER_OK)
       goto out;

    // run interpret
    result = interpret(instr_list);

    st_free(symtab);
    listFree(instr_list);
    fclose(source);

    return result;

out:
    st_free(symtab);
    fclose(source);

    return result;
}
