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

#include "instrlist.h"
#include "parser.h"
#include "symtab.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        return 99;
    }

    FILE *source;

    if ((source = fopen(argv[1], "r")) == NULL) {
        return 99;
    }

    // initialize table of symbols
    symtab_t *symtab = malloc(sizeof(symtab_t));
    st_init(symtab);

    // DEBUG
    st_print(symtab);

    symtab_elem_t elem;
    
    elem.elem_type = ST_ELEMTYPE_VAR;
    elem.data_type = ST_DATATYPE_INT;
    elem.value.ival = 42;
    st_insert(symtab, elem);

    elem.elem_type = ST_ELEMTYPE_VAR;
    elem.data_type = ST_DATATYPE_DOUBLE;
    elem.value.dval = 42.42e42;
    st_insert(symtab, elem);

    elem.elem_type = ST_ELEMTYPE_VAR;
    elem.data_type = ST_DATATYPE_STRING;
    elem.value.strval = "asdf";
    st_insert(symtab, elem);

    st_print(symtab);

    tListOfInstr *ilist = malloc(sizeof(tListOfInstr));
    listInit(ilist);

    // table of symbols == NULL (no table implemented yet)
    int parse_result = parse(ilist, symtab, source);

    // check error code

    // interpret code

    // free table of symbols
    // free instruction list
    fclose(source);

    return 0;
}
