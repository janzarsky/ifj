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

#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

int bool_expr();
int math_expr(int *type);
int string_concat();

long int int_from_token(char *token_data);
double *double_from_token(char *token_data);
char *string_from_token(char *token_data);

#endif
