#include "scanner.h"
#include "instrlist.h"

#ifndef _PARSER_H
#define _PARSER_H

int parse(tListOfInstr * ilist, void * symtab , FILE* source);

#endif
