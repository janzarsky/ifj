#include "scanner.h"
#include "instrlist.h"
#include "symtab.h"

#ifndef _PARSER_H
#define _PARSER_H

int parse(tListOfInstr * ilist, symtab_t * symtab , FILE* source);

#endif
