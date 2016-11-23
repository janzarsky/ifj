#ifndef _PARSER_TEST_H
#define _PARSER_TEST_H

#define END_STATEMENT 66 //just to know that enter is over
#define SYNTAX_OK 100
#define SYNTAX_ERROR 99

//defines from "string.c"
#define STR_ERROR  -1
#define STR_SUCCESS 0


int return_args();
int statement_list();
int func_var();
int program();
int equal();
int class_dec();
int func_args();
int func_args_list();
int func_params();
int func_params_list();

#endif
