#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#define SEMANTIC_ERROR 98

enum type_t { TYPE_ERROR, TYPE_NONE, TYPE_VOID, TYPE_INT, TYPE_DOUBLE, TYPE_STRING,
    TYPE_BOOL, TYPE_MAX };

int bool_expr();
int math_expr(int *type);


#endif
