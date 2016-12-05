#ifndef _PARSER_TEST_H
#define _PARSER_TEST_H

#include "symtab.h"

#define END_STATEMENT 66 //just to know that enter is over

void set_symtable(symtab_t *table);
void set_symtable_local(symtab_t *table);

int program();

char * str_conc(char * class_name, char * var_name);

#endif
