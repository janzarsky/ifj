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
#ifndef _PARSER_H
#define _PARSER_H

#include "symtab.h"

void set_symtable(symtab_t *table);

int program();

char * str_conc(char * class_name, char * var_name);

#endif
