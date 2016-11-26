#include <stdio.h>
#include <stdlib.h>
#include "parser_test.h"
#include "instrlist.h"
#include "expr_parser.h"
#include "symtab.h"
#include "scanner.h"

#define DEBUG

int return_args();
int statement_list();
int func_var();
int assign();
int class_dec();
int func_args();
int func_args_list();
int func_params();
int func_params_list();

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

		// 2) <prog>      -> CLASS ID LEFT_VINCULUM <class-dec>  // declaration of class
// int statement[] = {CLASS, ID, LEFT_VINCULUM,STATIC, INT, ID , LEFT_BRACKET, INT, ID, COMMA, DOUBLE, ID,COMMA,STRING,ID, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, RIGHT_VINCULUM, END_OF_FILE}; 
//int statement[] = {CLASS, ID, LEFT_VINCULUM,STATIC, INT, ID, ASSIGN, ID, LEFT_BRACKET, ID,COMMA, STRING_LITERAL, RIGHT_BRACKET,  SEMICOLON, RIGHT_VINCULUM, END_STATEMENT}; 

		// 3) <prog>      -> STATIC [INT/DOUBLE/SRING/VOID] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
// int statement[] = { STATIC, INT, ID , LEFT_BRACKET, INT, ID, COMMA, DOUBLE, ID,COMMA,STRING,ID, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, END_OF_FILE}; 
static int pruchod = 0;
symtab_t *tabulka;
symtab_t *local_tabulka;
int token;
char *token_data;
symtab_elem_t * item;
symtab_elem_t * current_function;
symtab_elem_t * current_class;
char *id;
int type;
extern tListOfInstr *instr_list;


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
	if(pruchod == 0){
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
								 		if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RUN){
								 			if(st_find(tabulka,token_data) == NULL){
								 				current_function = st_add(tabulka,token_data);
								 				current_function->data_type = ST_DATATYPE_VOID;
								 				current_function->elem_type = ST_ELEMTYPE_FUN;
								 				#ifdef DEBUG
								 				printf("\x1b[31m" "\n---------------------------------------------------------------------------------------\n");
								 				printf(  "new function = %s\n" ,current_function->id  );
								 				printf( "---------------------------------------------------------------------------------------\n" "\x1b[0m");
								 				#endif
								 			}
								 			else{
								 				;
								 				//FIXME make new ramec for recursive RUN call
								 				;
								 			}
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
										}			
							if(token == LEX_ERROR)
								return LEX_ERROR;						
							return SYNTAX_ERROR;
							break;	
	// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
					case ID: 

							if( st_find(tabulka,token_data) != NULL )
								return SEMANTIC_ERROR;
							current_class = st_add(tabulka, token_data);
							current_class->elem_type = ST_ELEMTYPE_CLASS;
							current_class->declared = current_class->initialized = 1;

							#ifdef DEBUG
							printf("\x1b[33m" "\n---------------------------------------------------------------------------------------\n");
							printf(  "new  class = %s\n" ,current_class->id  );
							printf( "---------------------------------------------------------------------------------------\n" "\x1b[0m");
							#endif

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
	//	3) <prog>      -> END_OF_FILE	
			case END_OF_FILE:
				pruchod++;	
				return SYNTAX_OK;
				break;
		}
		return SYNTAX_ERROR;
	}
	else{
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
							 		if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == RUN){
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
									}			
						if(token == LEX_ERROR)
							return LEX_ERROR;						
						return SYNTAX_ERROR;
						break;	
	// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
					case ID: 
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
	//	3) <prog>      -> END_OF_FILE	
			case END_OF_FILE:
				pruchod++;	
				return SYNTAX_OK;
				break;
		}
		return SYNTAX_ERROR;
	}
}

/*st-list>//function body
		1) <st-list>   -> RETURN <return-args> <st-list> //return in function
		2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> LEFT_VINCULUM <st-list> <st-list>//cycle while
		3) <st-list>   -> IF LEFT_BRACKET <bool-expr> LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list> //if-else statement
		4) <st-list>   -> RIGHT_VINCULUM	// end of sequence
		6) <st-list>   -> [INT/DOUBLE/SRING] ID [ SEMICOLON / ASSIGN <assign>] <st-list>
		7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list> //it can be function call OR inicialization of var*/
int statement_list(){
    printf("PARSER: function statement_list()\n");
	int result;
	int prev_token;
	tListItem * label1;
	tListItem * label2;
	tListItem * temp_item_list;
	tListItem * temp_item_list_else;
	symtab_elem_t * local_item;

//pruchod 1
	if(pruchod == 0){
		int bracket_counter=0;

		while(bracket_counter != -1){
			if( (token = get_next_token(&token_data)) == LEX_ERROR)
				return LEX_ERROR;
			if(token == RIGHT_VINCULUM)
				bracket_counter--;
			if(token == LEFT_VINCULUM)
				bracket_counter++;
		}
		return SYNTAX_OK;
	}

//pruchod 2	
	else{

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
		// 2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> LEFT_VINCULUM <st-list> <st-list>			
				case WHILE:

					add_instr(IN_LABEL,NULL,NULL,NULL); //generate label for start of WHILE cyclus
					label1 = (tListItem *)listGetPointerLast(instr_list);

					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET){
						if ( (result = bool_expr()) != SYNTAX_OK)
							return result;

						add_instr(IN_IFNGOTO,NULL,NULL,NULL);	//IF FALSE GOTO end of WHILE cyclus
						temp_item_list = (tListItem *)listGetPointerLast(instr_list);

		                if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
		                    if ( (result = statement_list()) != SYNTAX_OK)
		                        return result;

		                    add_instr(IN_GOTO,NULL,NULL,&(label1->instruction)); // GOTO start of WHILE cyclus
		                    add_instr(IN_LABEL,NULL,NULL,NULL);	//generate label for end of WHILE cyclus
		                    label2 = (tListItem *)listGetPointerLast(instr_list);
		                    temp_item_list->instruction.addr3 = &(label2->instruction);

		                    if ( (result = statement_list()) != SYNTAX_OK)
		                        return result;
		                    else return SYNTAX_OK;
		                }
					}
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;
					break;
		// 3) <st-list>   -> IF LEFT_BRACKET <bool-expr> LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list>			
				case IF:
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET){
						if ( (result = bool_expr()) != SYNTAX_OK)
							return result;

						add_instr(IN_IFNGOTO,NULL,NULL,NULL); //IF FALSE GOTO ELSE branch
						temp_item_list = (tListItem *)listGetPointerLast(instr_list);

						if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){
							if ( (result = statement_list()) != SYNTAX_OK)
								return result;

							add_instr(IN_GOTO,NULL,NULL,NULL);	// GOTO end of if-else statement
							temp_item_list_else = (tListItem *)listGetPointerLast(instr_list);

							if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ELSE)
								if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){

									add_instr(IN_LABEL,NULL,NULL,NULL);	//generate label for start of ELSE branch
									label1 = (tListItem *)listGetPointerLast(instr_list);
									temp_item_list->instruction.addr3 = &(label1->instruction);

									if ( (result = statement_list()) != SYNTAX_OK)
										return result;

									add_instr(IN_LABEL,NULL,NULL,NULL);	//generate label for end of if-else statement
									label2 = (tListItem *)listGetPointerLast(instr_list);
									temp_item_list_else->instruction.addr3 = &(label2->instruction);

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
		// 6) <st-list>   -> [INT/DOUBLE/SRING] ID [ SEMICOLON / ASSIGN <assign>] <st-list>
				case INT:
				case DOUBLE:
				case STRING:
					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID)

							if(st_find(local_tabulka,token_data) != NULL)
								return SEMANTIC_ERROR;
							local_item = item = st_add(local_tabulka, token_data);
							#ifdef DEBUG
							printf("\x1b[32m" "\n---------------------------------------------------------------------------------------\n");
							printf(  "new local token = %s\n" ,item->id  );
							printf( "---------------------------------------------------------------------------------------\n" "\x1b[0m");
							#endif
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
							item->local_table = local_tabulka; 
							if( (result = statement_list()) != SYNTAX_OK){
								return result;
							}
							else{
								return SYNTAX_OK;
							}
						}
						else if(token == ASSIGN){
							if( (result = assign()) != SYNTAX_OK ){

								add_instr(IN_MOVSTACK,NULL,NULL,(void*) local_item);

								return result;
							}
							else{
								if( (result = statement_list()) != SYNTAX_OK){
									return result;
								}
								else{
									return SYNTAX_OK;
								}
							}
						}
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;
					break;	
		 // 7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list>
				case ID:
					if ( (result = func_var()) != SYNTAX_OK){
						return result;
					}
					if ( (result = statement_list()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
					break;
			}
			return SYNTAX_ERROR;
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
// 		2) ASSIGN <assign> //its inicialization of var
int func_var(){
    printf("PARSER: function func_var()\n");
	int result;
	id = token_data;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) LEFT_BRACKET <func-args> SEMICOLON //it's function call	
		case LEFT_BRACKET:
			current_function = item;
			/*if( (current_function = st_find(tabulka, id)) == NULL){ //if function not in symtab we add it there.
				current_function = st_add(tabulka, id);
				current_function->elem_type = ST_ELEMTYPE_FUN;
				current_function->initialized = current_function->declared = 0;
			}*/
			if ( (result = func_args()) != SYNTAX_OK)
				return result;


			add_instr(IN_CALL,NULL,NULL, (void*)current_function); // instruction for FUNCTION CALL


			if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
// 2) ASSIGN <assign> //its inicialization of var			
		case ASSIGN:
			if ( (result = assign()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <return-args>
// 		1) <return-args> -> SEMICOLON (ONLY if we in VOID function)
// 		4) <return-args> -> <math-expr> SEMICOLON
int return_args(){
    printf("PARSER: function return_args()\n");
	int result;
	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) <return-args> -> SEMICOLON (ONLY if we in VOID function)		
		case SEMICOLON:

			if(current_function->data_type != ST_DATATYPE_VOID){
				return SEMANTIC_ERROR; //FIXME check return error type
			} 
			add_instr(IN_RETURN, NULL, NULL, NULL);
			return SYNTAX_OK;
			break;
		default:

			if(current_function->data_type == ST_DATATYPE_VOID){
				return 8; // return smth in void function
			}

			return_token(token, token_data);
			if( (result = math_expr(&type)) == SYNTAX_OK){


				if(current_function->data_type == ST_DATATYPE_DOUBLE && type == ST_DATATYPE_INT){
					add_instr(IN_CONV, NULL, NULL, NULL);
					type = ST_DATATYPE_DOUBLE;
				}

				if(current_function->data_type != type){
					return SEMANTIC_ERROR; //FIXME check return type
				}
				else{
					add_instr(IN_RETURN, NULL, NULL, NULL); //FIXME current function must be changed to previous function
				}

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


// <assign>	
// 		2) <assign>	   -> ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) SEMICOLON 
// 		3) <assign>	   -> ID [SEMICOLON/ <math-expr>]
// 		4) <assign>	   -> <math-expr> SEMICOLON
int assign(){
    printf("PARSER: function assign()\n");
	int result;
    int temp_token;
    char *temp_token_data;
    symtab_elem_t * temp_elem;
    char *end;


	if ( (token = get_next_token(&token_data)) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case ID:
            temp_token = token;
            temp_token_data = token_data;
            if( (temp_elem = st_find(local_tabulka,temp_token_data)) == NULL)
	            if( (temp_elem = st_find(tabulka,temp_token_data)) == NULL)
	            	return SEMANTIC_ERROR; //error number 3 

			if ( (token = get_next_token(&token_data)) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// 2) <assign>	   -> ID LEFT_BRACKET <func-args> SEMICOLON //function call			
				case LEFT_BRACKET: 
					if ( (result = func_args()) != SYNTAX_OK)
						return result;

						current_function = temp_elem;
				        add_instr(IN_CALL,NULL,NULL,(void *)current_function);


					if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == SEMICOLON)
						return SYNTAX_OK;
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;
// 3) <assign>	   -> ID SEMICOLON					
				case SEMICOLON:
					if(!(item->declared) ||  !(temp_elem->declared) || !(temp_elem->initialized) )
						return SEMANTIC_ERROR; //error number 3

					add_instr(IN_TAB_PUSH, (void *)temp_elem, NULL, NULL);

					switch(item->data_type){
						case ST_DATATYPE_INT:
								if(temp_elem->data_type != ST_DATATYPE_INT)
									return 4; 			//type compatibility error
								item->initialized = 1;
								break;
						case ST_DATATYPE_DOUBLE:
								if(temp_elem->data_type != ST_DATATYPE_DOUBLE){
									add_instr(IN_CONV,NULL, NULL, NULL);
								}
								item->initialized = 1;
								break;
						case ST_DATATYPE_STRING:
								if(temp_elem->data_type != ST_DATATYPE_STRING)
									return 4; 			//type compatibility error
								item->initialized = 1;
								break;
						default:
							return SEMANTIC_ERROR;
					}
						
					return SYNTAX_OK;
					break;
// 3) <assign>	   -> ID ...-> <math-expr>					
                default:
                    return_token(token, token_data);
                    return_token(temp_token, temp_token_data);

                    result = math_expr(&type);

                    if(result == SYNTAX_OK){
                        if(item->declared ){

                        	switch(item->data_type){
                        		case ST_DATATYPE_INT:
                        				if(type != ST_DATATYPE_INT)
                        					return 4; 			//type compatibility error
                        				break;
                        		case ST_DATATYPE_DOUBLE:
                        				if(type != ST_DATATYPE_DOUBLE){
                        					add_instr(IN_CONV, NULL, NULL, NULL);
                        				}
                        				break;
                        		case ST_DATATYPE_STRING:
                        				if(type != ST_DATATYPE_STRING)
                        					return 4; 			//type compatibility error
                        				break;
                        		default:
                        			return SEMANTIC_ERROR; 
                        	}
							item->initialized = 1;
							return SYNTAX_OK;
						}
						else
							return SEMANTIC_ERROR; //error type 3
                    }
                    return result;
            }
        default:
            return_token(token, token_data);

            result = math_expr(&type);
            if(item->declared ){

            	switch(item->data_type){
            		case ST_DATATYPE_INT:
            				if(type != ST_DATATYPE_INT)
            					return 4; 			//type compatibility error
            				item->initialized = 1;
            				break;
            		case ST_DATATYPE_DOUBLE:
            				if(type != ST_DATATYPE_DOUBLE){
            					add_instr(IN_CONV, NULL, NULL, NULL);
            				}
            				item->initialized = 1;
            				break;
            		case ST_DATATYPE_STRING:
            				if(type != ST_DATATYPE_STRING)
            					return 4; 			//type compatibility error
            				item->initialized = 1;
            				break;
            		default:
            			return SEMANTIC_ERROR; 
            	}

					item->initialized = 1;
			}
			else
				return SEMANTIC_ERROR; //error type 3
            

			return result;
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
//               [SEMICOLON/ ASSIGN <math-expr>/LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>] <class-dec>
// 		2) <class-dec> -> RIGHT_VINCULUM
// 		3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>
int class_dec(){
    printf("PARSER: function class_dec()\n");
	int result;
	int prev_token;
	symtab_elem_t * find;


	if(pruchod == 0){
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

							if( (find = st_find(tabulka,token_data)) != NULL){
								if(find->declared == 1 || find->initialized == 1)
									return SEMANTIC_ERROR; //redeclaration of existing symbol
								else{
									find->declared = 1; //(must NOT happen) if ID exist in global table but not declared and not initialized 
								}
							}
							else{
								item = st_add(tabulka,token_data);
								item->declared = 1;
								#ifdef DEBUG
								printf("\x1b[31m" "\n---------------------------------------------------------------------------------------\n");
								printf(  "new global id = %s\n" ,item->id  );
								printf( "---------------------------------------------------------------------------------------\n" "\x1b[0m");
								#endif
							}
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
									

									item->initialized = 0;
									item->elem_type = ST_ELEMTYPE_VAR;


									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID  ASSIGN <math-expr> <class-dec>								
								case ASSIGN:

									item->elem_type = ST_ELEMTYPE_VAR;

									if ( (result = math_expr(&type)) != SYNTAX_OK)
										return result;
									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>							
								case LEFT_BRACKET:

									current_function = item;
									current_function->elem_type = ST_ELEMTYPE_FUN;

									if ( (result = func_params()) != SYNTAX_OK)
										return result;
									if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){

										current_function->initialized = 1;

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
						if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID){

							if( (find = st_find(tabulka,token_data)) != NULL){
								if(find->declared == 1 || find->initialized == 1)
									return SEMANTIC_ERROR; //redeclaration of existing symbol
								else{
									find->declared = 1;
								}
							}
							else{
								current_function = st_add(tabulka,token_data);
								current_function->elem_type = ST_ELEMTYPE_FUN;
								current_function->data_type = ST_DATATYPE_VOID;
								current_function->declared =  1;
								#ifdef DEBUG
								printf("\x1b[31m" "\n---------------------------------------------------------------------------------------\n");
								printf(  "new global function = %s\n" ,current_function->id  );
								printf( "---------------------------------------------------------------------------------------\n" "\x1b[0m");
								#endif
							}
							id = token_data;
							if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_BRACKET){
								if ( (result = func_params()) != SYNTAX_OK)
									return result;
								if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == LEFT_VINCULUM){

									current_function->initialized = 1;

									if ( (result = statement_list()) != SYNTAX_OK)
										return result;
									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
								}
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

	//pruchod 2
	else{
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

							item = st_find(tabulka,token_data);

							if ( (token = get_next_token(&token_data)) == LEX_ERROR )
								return LEX_ERROR;
							switch(token){
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID SEMICOLON <class-dec>							
								case SEMICOLON:
									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID  ASSIGN <math-expr> <class-dec>								
								case ASSIGN:
									if ( (result = math_expr(&type)) != SYNTAX_OK)
										return result;

									add_instr(IN_MOVSTACK,NULL,NULL,(void *)item);

									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>							
								case LEFT_BRACKET:
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
						if ( (token = get_next_token(&token_data)) != LEX_ERROR && token == ID){
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
}
