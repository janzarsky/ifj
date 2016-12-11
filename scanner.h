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

#ifndef _SCANNER_H
#define _SCANNER_H

#include "string.h"

void setSourceFile(FILE *f);
int get_next_token(char **buffer);
void return_token(int token, char *buffer);

#endif
