#ifndef _PARSER_TEST_H
#define _PARSER_TEST_H

#include "symtab.h"

#define END_STATEMENT 66 //just to know that enter is over
#define SYNTAX_OK 100
#define SYNTAX_ERROR 99


void set_symtable(symtab_t *table);
void set_symtable_local(symtab_t *table);

int program();

#endif
