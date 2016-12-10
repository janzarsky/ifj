#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

int bool_expr();
int math_expr(int *type);
int string_concat();

long int int_from_token(char *token_data);
double *double_from_token(char *token_data);
char *string_from_token(char *token_data);

#endif
