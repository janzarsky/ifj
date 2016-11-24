#include <stdio.h>
#include "parser_test.h"
#include "instrlist.h"
#include "expr_parser.h"
#include "string.h"
#include "symtab.h"
#include "scanner.h"

/*void generateInstruction(int instType, void *addr1, void *addr2, void *addr3)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}*/

//		1) <prog> 	   -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
// int statement[] = {CLASS, MAIN, LEFT_VINCULUM, STATIC, VOID, RUN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, RIGHT_VINCULUM, END_OF_FILE}; 

		// 2) <prog>      -> CLASS ID LEFT_VINCULUM <class-dec>  // declaration of clas
// int statement[] = {CLASS, ID, LEFT_VINCULUM,STATIC, INT, ID , LEFT_BRACKET, INT, ID, COMMA, DOUBLE, ID,COMMA,STRING,ID, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, RIGHT_VINCULUM, END_OF_FILE}; 
int statement[] = {CLASS, ID, LEFT_VINCULUM,STATIC, INT, ID, EQUAL, ID, LEFT_BRACKET, ID,COMMA, STRING_LITERAL, RIGHT_BRACKET,  SEMICOLON, RIGHT_VINCULUM, END_STATEMENT}; 

		// 3) <prog>      -> STATIC [INT/DOUBLE/SRING/VOID] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
// int statement[] = { STATIC, INT, ID , LEFT_BRACKET, INT, ID, COMMA, DOUBLE, ID,COMMA,STRING,ID, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, END_OF_FILE}; 
symtab_t *tabulka;
symtab_t *local_tabulka;
int token;
char *token_data;
symtab_elem_t * item;
string * id;
int type;

void set_symtable(symtab_t *table) {
    tabulka = table;
}

void set_symtable_local(symtab_t *table) {
    local_tabulka = table;
}

// 1) <prog>  -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM <prog>
// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
//-- 3) <prog> -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>	//declaration and initialization of function
// 3) <prog>  -> END_OF_FILE
int program(){
    printf("PARSER: function program()\n");
	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case CLASS:
			if ( (token = get_next_token(&token_data)) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// 1) <prog>  -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM <prog>
				case MAIN:
						if ( (token = get_next_token(&token_data)) != LEX_ERROR  && token == LEFT_VINCULUM)
							 if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == STATIC)
							 	if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == VOID)
							 		if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RUN)
									 	if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET)
										 	if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RIGHT_BRACKET)
											 	if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
											 		if( (result = statement_list()) != SYNTAX_OK)
											 			return result;
											 		else
													 	if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RIGHT_VINCULUM){
													 		if( (result = program()) != SYNTAX_OK)
													 			return result;
													 		else
														 		return SYNTAX_OK;
														}	 	
												}
						if(token == LEX_ERROR)
							return LEX_ERROR;						
						return SYNTAX_ERROR;
						break;	
// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
				case ID: 

						if(st_find(tabulka,token_data) != NULL)
							return SEMANTIC_ERROR;
						item = st_add(tabulka, token_data);
						item->elem_type = ST_ELEMTYPE_CLASS;
						item->declared = item->initialized = 1;

						if ( (token = get_next_token(&token_data)) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = class_dec()) != SYNTAX_OK)
					 			return result;
					 		if( (result = program()) != SYNTAX_OK)
					 			return result;
					 		else
								return SYNTAX_OK;
						}
						if(token == LEX_ERROR)
							return LEX_ERROR;
						return SYNTAX_ERROR;
						break;
			}
			return SYNTAX_ERROR;
// 3) <prog>      -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func_params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
		/*case STATIC:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR  && (token == VOID || token == INT || token == DOUBLE || token == STRING) )
				if ( (token = get_next_token(&token_data)) != LEX_ERROR  && token == ID)
					if ( (token = get_next_token(&token_data)) != LEX_ERROR  && token == LEFT_BRACKET){
						if( (result = func_params()) != SYNTAX_OK)
					 			return result;
					 	if ( (token = get_next_token(&token_data)) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = statement_list()) != SYNTAX_OK)
					 			return result;
					 		//else return SYNTAX_OK;
					 		if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == END_STATEMENT)
								return SYNTAX_OK;
					 	}
					}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;*/
//	3) <prog>      -> END_OF_FILE	
		case END_OF_FILE:	
			return SYNTAX_OK;
	}
	return SYNTAX_ERROR;
}

/*st-list>//function body
		1) <st-list>   -> RETURN <return-args> <st-list> //return in function
		2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> <st-list>//cycle while
		3) <st-list>   -> IF LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list> //if-else statement
		4) <st-list>   -> RIGHT_VINCULUM	// end of sequence
		6) <st-list>   -> [INT/DOUBLE/SRING] ID [ SEMICOLON / EQUAL <equal>] <st-list>
		7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list> //it can be function call OR inicialization of var*/
int statement_list(){
    printf("PARSER: function statement_list()\n");
	int result;
	int prev_token;
	if ( (prev_token = token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) <st-list>   -> RETURN <return-args> <st-list> 		
		case RETURN:
			if((result = return_args()) != SYNTAX_OK)
				return result;
			if((result = statement_list()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
// 2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> <st-list>			
		case WHILE:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET){
				if ( (result = bool_expr(&type)) != SYNTAX_OK)
					return result;
				if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RIGHT_BRACKET)
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						else return SYNTAX_OK;
					}
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
// 3) <st-list>   -> IF LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list>			
		case IF:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET){
				if ( (result = bool_expr(&type)) != SYNTAX_OK)
					return result;
				if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RIGHT_BRACKET)
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ELSE)
							if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
								if ( (result = statement_list()) != SYNTAX_OK)
									return result;
								if ( (result = statement_list()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
							}
					}
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
// 4) <st-list>   -> RIGHT_VINCULUM			
		case RIGHT_VINCULUM:
			return SYNTAX_OK;
			break;
// 6) <st-list>   -> [INT/DOUBLE/SRING] ID [ SEMICOLON / EQUAL <equal>] <st-list>
		case INT:
		case DOUBLE:
		case STRING:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID)

					if(st_find(local_tabulka,token_data) != NULL)
						return SEMANTIC_ERROR;
					item = st_add(local_tabulka, token_data);
					switch(prev_token){
						case INT:
							item->data_type = ST_DATATYPE_INT;
							break;
						case DOUBLE:
							item->data_type = ST_DATATYPE_DOUBLE;
							break;
						case STRING:
							item->data_type = ST_DATATYPE_STRING;
							break;
					}
					item->declared = 1;
					item->initialized = 0;

				if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON){
					item->elem_type = ST_ELEMTYPE_VAR;
//???????????????????????????????????????????????????????????? item->local_table = 
				}
				else if(token == EQUAL){
					if( (result = equal()) != SYNTAX_OK )
						return result;
					else return SYNTAX_OK;
				}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;	
 // 7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list>
		case ID:
			if ( (result = func_var()) != SYNTAX_OK)
					return result;
			if ( (result = statement_list()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <func-params>//function DECLARATION + INIT
// 		1) <func-params> -> RIGHT_BRACKET
// 		2) <func-params> -> [INT/DOUBLE/SRING] ID <func-params-list>
int func_params(){
    printf("PARSER: function func_params()\n");
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	int result;
	switch(token){
// 1) <func-params> -> RIGHT_BRACKET		
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
// 2) <func-params> -> [INT/DOUBLE/SRING] ID <func-params-list>			
		case INT:
		case DOUBLE:
		case STRING:

						//FIXME add insert to symTab

			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID){
				if ( (result = func_params_list()) != SYNTAX_OK)
					return result;
				else return SYNTAX_OK;
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
	}
	return SYNTAX_ERROR;
}

// <func-params-list>
// 		1) <func-params-list> -> RIGHT_BRACKET
// 		2) <func-params-list> -> COMMA [INT/DOUBLE/SRING] ID <func-params-list>
int func_params_list(){
    printf("PARSER: function func_params_list()\n");
	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case COMMA:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && (token == INT || token == DOUBLE || token == STRING))
				if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID){

					//FIXME add insert to symTable to function parameters

					if ( (result = func_params_list()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
				}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
	}
	return SYNTAX_ERROR;
}

// <func_var>
// 		1) LEFT_BRACKET <func-args> SEMICOLON //its function call
// 		2) EQUAL <equal> //its inicialization of var
int func_var(){
    printf("PARSER: function func_var()\n");
	int result;
	id = token_data;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) LEFT_BRACKET <func-args> SEMICOLON //it's function call	
		case LEFT_BRACKET:
			
			
			if(st_find(tabulka, id->str) == NULL){
				item = st_add(tabulka, id->str);
				item->elem_type = ST_ELEMTYPE_FUN;
				item->initialized = item->declared = 0;
			}
			//FIXME instruction for calling a function

			if ( (result = func_args()) != SYNTAX_OK)
				return result;
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
// 2) EQUAL <equal> //its inicialization of var			
		case EQUAL:
			if ( (result = equal()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <return-args>
// 		1) <return-args> -> SEMICOLON (ONLY if we in VOID function)
//-- 		2) <return-args> -> ID SEMICOLON
//-- 		3) <return-args> -> [INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] SEMICOLON
// 		4) <return-args> -> <math-expr> SEMICOLON
int return_args(){
    printf("PARSER: function return_args()\n");
	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) <return-args> -> SEMICOLON (ONLY if we in VOID function)		
		case SEMICOLON:

			//FIXME check if we are in VOID function

			return SYNTAX_OK;
			break;
// // 2) <return-args> -> ID SEMICOLON			
// 		case ID:
// 			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
// 				return SYNTAX_OK;
// 			if(token == LEX_ERROR)
// 				return LEX_ERROR;
// 			return SYNTAX_ERROR;	
// 			break;
// // 3) <return-args> -> [INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] SEMICOLON			
// 		case INT_LITERAL:
// 		case DOUBLE_LITERAL:
// 		case STRING_LITERAL:
// 			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
// 				return SYNTAX_OK;
// 			if(token == LEX_ERROR)
// 				return LEX_ERROR;
// 			return SYNTAX_ERROR;	
// 			break;
		default:
			return_token(token, token_data);
			if( (result = math_expr(&type)) == SYNTAX_OK){

				//FIXME insert to symbol table
				//FIXME check function return type

				if(token == SEMICOLON)
					return SYNTAX_OK;
				else 
					return SYNTAX_ERROR;
			}
			else{
				return result;
			}
	}
	return SYNTAX_ERROR;
}


// <equal>	
//-- 		1) <equal>	   -> [INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] SEMICOLON
// 		2) <equal>	   -> ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) SEMICOLON 
// 		3) <equal>	   -> ID [SEMICOLON/EQUAL <equal>]
// 		4) <equal>	   -> <math-expr> SEMICOLON
int equal(){
    printf("PARSER: function equal()\n");
	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		/*case INT_LITERAL:
		case DOUBLE_LITERAL:
		case STRING_LITERAL:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;*/
		case ID:
			if ( (token = get_next_token(&token_data)) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// 2) <equal>	   -> ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) SEMICOLON //function call			
				case LEFT_BRACKET: 

						//FIXME add insert function to symTab 

					if ( (result = func_args()) != SYNTAX_OK)
						return result;
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
						return SYNTAX_OK;
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;
// 3) <equal>	   -> ID SEMICOLON					
				case SEMICOLON:

						//FIXME add insert var to symTab

					return SYNTAX_OK;
					break;
// 3) <equal>	   -> ID EQUAL <equal>					
				case EQUAL:	

						//FIXME add insert var to symTab

					if ( (result = equal()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
					break;
				default:
					return_token(token, token_data);
					if( (result = math_expr(&type)) == SYNTAX_OK){

						//FIXME insert to symbol table

						if(token == SEMICOLON)
							return SYNTAX_OK;
						else 
							return SYNTAX_ERROR;
					}
					else{
						return result;
					}	
			}
			return SYNTAX_ERROR;
	}
	return SYNTAX_ERROR;
}

// <func-args>//function CALL
		// 1) <func-args> -> RIGHT_BRACKET
		// 2) <func-args> -> [ID/INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] <func-args-list>
int func_args(){
    printf("PARSER: function func_args()\n");
	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case ID:
		case INT_LITERAL:
		case DOUBLE_LITERAL:
		case STRING_LITERAL:

		//FIXME function arguments

			if ( (result = func_args_list()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <func-args-list>
// 		1) <func-args-list> -> RIGHT_BRACKET
// 		2) <func-args-list> -> COMMA [ID/INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] <func-args-list>
int func_args_list(){
    printf("PARSER: function func_args_list()\n");

	//FIXME function arguments

	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case COMMA:
			if ( (token = get_next_token(&token_data)) != LEX_ERROR && 
				(token == INT_LITERAL || token == DOUBLE_LITERAL || token == STRING_LITERAL || token == ID)){

				if ( (result = func_args_list()) != SYNTAX_OK)
					return result;
				else return SYNTAX_OK;
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
	}
	return SYNTAX_ERROR;
}

// <class-dec>
// 		1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID \n
//               [SEMICOLON/ EQUAL <equal>/LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>] <class-dec>
// 		2) <class-dec> -> RIGHT_VINCULUM
// 		3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>
int class_dec(){
    printf("PARSER: function class_dec()\n");
	int result;
	int prev_token;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case STATIC:
			if ( (token = get_next_token(&token_data)) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){				
				case INT:
				case DOUBLE:
				case STRING:
					prev_token=token;
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID){

						if(st_find(tabulka,token_data) != NULL){
							return SEMANTIC_ERROR; //redeclaration of existing symbol
						}
						item = st_add(tabulka,token_data);
						switch(prev_token){
							case INT:
								item->data_type = ST_DATATYPE_INT;
								break;
							case DOUBLE:
								item->data_type = ST_DATATYPE_DOUBLE;
								break;
							case STRING:
								item->data_type = ST_DATATYPE_STRING;
								break;
						}

						if ( (token = get_next_token(&token_data)) == LEX_ERROR )
							return LEX_ERROR;
						switch(token){
// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID SEMICOLON <class-dec>							
							case SEMICOLON:
								

								item->declared = 1;
								item->initialized = 0;
								item->elem_type = ST_ELEMTYPE_VAR;


								if ( (result = class_dec()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
								break;
// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID  EQUAL <equal> <class-dec>								
							case EQUAL:

								item->declared = 1;
								item->elem_type = ST_ELEMTYPE_VAR;
								item->initialized = 1;

								if ( (result = equal()) != SYNTAX_OK)
									return result;
								if ( (result = class_dec()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
								break;
// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>							
							case LEFT_BRACKET:

								item->declared = 1;
								item->elem_type = ST_ELEMTYPE_FUN;
								item->initialized = 1;

								if ( (result = func_params()) != SYNTAX_OK)
									return result;
								if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
									if ( (result = statement_list()) != SYNTAX_OK)
										return result;
									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
								}
								if(token == LEX_ERROR)
									return LEX_ERROR;
								return SYNTAX_ERROR;	
								break;
						}
						return SYNTAX_ERROR;
					}
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;
// 3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>					
				case VOID:
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID)

						if(st_find(tabulka,token_data) != NULL){
							return SEMANTIC_ERROR; //redeclaration of existing symbol
						}
						item = st_add(tabulka,token_data);
						item->elem_type = ST_ELEMTYPE_FUN;
						item->data_type = ST_DATATYPE_VOID;
						item->declared = item->initialized = 1;

						id = token_data;
						if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET){
							if ( (result = func_params()) != SYNTAX_OK)
								return result;
							if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
								if ( (result = statement_list()) != SYNTAX_OK)
									return result;
								if ( (result = class_dec()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
							}
						}
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;		
			}
			return SYNTAX_ERROR;	
			break;
// 2) <class-dec> -> RIGHT_VINCULUM			
		case RIGHT_VINCULUM:
			return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}
