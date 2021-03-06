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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "instrlist.h"
#include "expr_parser.h"
#include "symtab.h"
#include "scanner.h"
#include "error_codes.h"

int return_args();
int statement_list();
int func_var();
int assign();
int class_dec();
int func_args();
int func_args_list();
int func_args_print();
int func_params();
int func_params_list();

static int pruchod = 0;
symtab_t *tabulka;
symtab_t *local_tabulka;
int token;
char *token_data;
symtab_elem_t * item;
symtab_elem_t * current_function;
symtab_elem_t * called_function;
symtab_elem_t * current_class;
symtab_elem_t * current_param;
char *id;
int type;
extern tListOfInstr *instr_list;
unsigned int run_counter=0;
bool is_empty_file = true;

void set_symtable(symtab_t *table) {
    tabulka = table;
}

char * str_conc(char * class_name, char * var_name){

	char * summ= malloc(sizeof(char) * (strlen(class_name) + strlen(var_name) + strlen(".") + 1)); 
	strcpy(summ, class_name);
	strcat(summ, ".");
	strcat(summ, var_name);

	return summ;
}

// 1) <prog>  -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM <prog>
// 1.1) <prog>  -> CLASS MAIN LEFT_VINCULUM  <class-dec> <prog>
// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
//-- 3) <prog> -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>	//declaration and initialization of function
// 3) <prog>  -> END_OF_FILE
int program(){
	int result;
//pruchod 1
	if(pruchod == 0){
		if ( (token = get_next_token(&token_data)) == ER_LEX )
			return ER_LEX;
		switch(token){
			case CLASS:
                is_empty_file = false;

				if ( (token = get_next_token(&token_data)) == ER_LEX )
					return ER_LEX;
				switch(token){
	// 1) <prog>  -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM <prog>
					case MAIN:

						if( st_find(tabulka,token_data) != NULL )
							return ER_SEM;
						current_class = st_add(tabulka, token_data);
						current_class->elem_type = ST_ELEMTYPE_CLASS;
						current_class->declared = 1;
                        current_class->initialized = 1;
                        current_class->is_global = 1;

						if ( (token = get_next_token(&token_data)) != ER_LEX  && token == LEFT_VINCULUM){
					 		if( (result = class_dec()) != ER_OK)
					 			return result;
					 		if(run_counter != 1)
					 			return ER_SEM; //error number 3 not defined RUN function in main	
					 		if( (result = program()) != ER_OK)
					 			return result;
					 		else
								return ER_OK;
						}
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;
						break;	
	// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
					case ID: 

							if( st_find(tabulka,token_data) != NULL )
								return ER_SEM;
							current_class = st_add(tabulka, token_data);
							current_class->elem_type = ST_ELEMTYPE_CLASS;
							current_class->declared = current_class->initialized = 1;

							if ( (token = get_next_token(&token_data)) != ER_LEX  && token == LEFT_VINCULUM){
						 		if( (result = class_dec()) != ER_OK)
						 			return result;
						 		if( (result = program()) != ER_OK)
						 			return result;
						 		else
									return ER_OK;
							}
							if(token == ER_LEX)
								return ER_LEX;
							return ER_SYNTAX;
							break;
				}
				return ER_SYNTAX;
	//	3) <prog>      -> END_OF_FILE	
			case END_OF_FILE:
				pruchod++;
                
                if (is_empty_file)
                    return ER_SEM;

				return ER_OK;
				break;
		}
		return ER_SYNTAX;
	}
//pruchod 2
	else{
		if ( (token = get_next_token(&token_data)) == ER_LEX )
			return ER_LEX;
		switch(token){
			case CLASS:
				if ( (token = get_next_token(&token_data)) == ER_LEX )
					return ER_LEX;
				switch(token){
	// 1) <prog>  -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM <prog>
					case MAIN:
                        current_class = st_find(tabulka, "Main");

                        if (current_class == NULL) {
                            return ER_INTERN;
                        }
                        else if (current_class->elem_type != ST_ELEMTYPE_CLASS) {
                            return ER_INTERN;
                        }
						/*if ( (token = get_next_token(&token_data)) != ER_LEX  && token == LEFT_VINCULUM)
							 if ( (token = get_next_token(&token_data)) != ER_LEX && token == STATIC)
							 	if ( (token = get_next_token(&token_data)) != ER_LEX && token == VOID)
							 		if ( (token = get_next_token(&token_data)) != ER_LEX && token == RUN){
									 	if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET)
										 	if ( (token = get_next_token(&token_data)) != ER_LEX && token == RIGHT_BRACKET)
											 	if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){
											 		if( (result = statement_list()) != ER_OK)
											 			return result;
											 		else
													 	if ( (token = get_next_token(&token_data)) != ER_LEX && token == RIGHT_VINCULUM){
													 		if( (result = program()) != ER_OK)
													 			return result;
													 		else
														 		return ER_OK;
														}	 	
												}
									}*/			
						if ( (token = get_next_token(&token_data)) != ER_LEX  && token == LEFT_VINCULUM){
					 		if( (result = class_dec()) != ER_OK)
					 			return result;
					 		if( (result = program()) != ER_OK)
					 			return result;
					 		else
								return ER_OK;
						}
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;
						break;
	// 2) <prog>  -> CLASS ID LEFT_VINCULUM <class-dec> <prog> // declaration of class
					case ID: 
                        current_class = st_find(tabulka, token_data);

                        if (current_class == NULL)
                            return ER_INTERN;
                        else if (current_class->elem_type != ST_ELEMTYPE_CLASS)
                            return ER_INTERN;

						if ( (token = get_next_token(&token_data)) != ER_LEX  && token == LEFT_VINCULUM){
					 		if( (result = class_dec()) != ER_OK)
					 			return result;
					 		if( (result = program()) != ER_OK)
					 			return result;
					 		else
								return ER_OK;
						}
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;
						break;
				}
				return ER_SYNTAX;
	//	3) <prog>      -> END_OF_FILE	
			case END_OF_FILE:
				pruchod++;	
				return ER_OK;
				break;
		}
		return ER_SYNTAX;
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
			if( (token = get_next_token(&token_data)) == ER_LEX)
				return ER_LEX;

			if (token == RIGHT_VINCULUM)
				bracket_counter--;
			else if (token == LEFT_VINCULUM)
				bracket_counter++;
		}
		return ER_OK;
	}

//pruchod 2	
	else{

			if ( (prev_token = token = get_next_token(&token_data)) == ER_LEX )
				return ER_LEX;
			switch(token){
		// 1) <st-list>   -> RETURN <return-args> <st-list> 		
				case RETURN:
					if((result = return_args()) != ER_OK)
						return result;
					if((result = statement_list()) != ER_OK)
						return result;
					else return ER_OK;
					break;
		// 2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> LEFT_VINCULUM <st-list> <st-list>			
				case WHILE:

					add_instr(IN_LABEL,NULL,NULL,NULL); //generate label for start of WHILE cyclus
					label1 = (tListItem *)listGetPointerLast(instr_list);

					if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET){
						if ( (result = bool_expr()) != ER_OK)
							return result;

						add_instr(IN_IFNGOTO,NULL,NULL,NULL);	//IF FALSE GOTO end of WHILE cyclus
						temp_item_list = (tListItem *)listGetPointerLast(instr_list);

		                if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){
		                    if ( (result = statement_list()) != ER_OK)
		                        return result;

		                    add_instr(IN_GOTO,NULL,NULL,&(label1->instruction)); // GOTO start of WHILE cyclus
		                    add_instr(IN_LABEL,NULL,NULL,NULL);	//generate label for end of WHILE cyclus
		                    label2 = (tListItem *)listGetPointerLast(instr_list);
		                    temp_item_list->instruction.addr3 = &(label2->instruction);

		                    if ( (result = statement_list()) != ER_OK)
		                        return result;
		                    else return ER_OK;
		                }
					}
					if(token == ER_LEX)
						return ER_LEX;
					return ER_SYNTAX;
					break;
		// 3) <st-list>   -> IF LEFT_BRACKET <bool-expr> LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list>			
				case IF:
					if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET){
						if ( (result = bool_expr()) != ER_OK)
							return result;

						add_instr(IN_IFNGOTO,NULL,NULL,NULL); //IF FALSE GOTO ELSE branch
						temp_item_list = (tListItem *)listGetPointerLast(instr_list);

						if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){
							if ( (result = statement_list()) != ER_OK)
								return result;

							add_instr(IN_GOTO,NULL,NULL,NULL);	// GOTO end of if-else statement
							temp_item_list_else = (tListItem *)listGetPointerLast(instr_list);

							if ( (token = get_next_token(&token_data)) != ER_LEX && token == ELSE)
								if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){

									add_instr(IN_LABEL,NULL,NULL,NULL);	//generate label for start of ELSE branch
									label1 = (tListItem *)listGetPointerLast(instr_list);
									temp_item_list->instruction.addr3 = &(label1->instruction);

									if ( (result = statement_list()) != ER_OK)
										return result;

									add_instr(IN_LABEL,NULL,NULL,NULL);	//generate label for end of if-else statement
									label2 = (tListItem *)listGetPointerLast(instr_list);
									temp_item_list_else->instruction.addr3 = &(label2->instruction);

									if ( (result = statement_list()) != ER_OK)
										return result;
									else return ER_OK;
								}
						}
					}
					if(token == ER_LEX)
						return ER_LEX;
					return ER_SYNTAX;
					break;
		// 4) <st-list>   -> RIGHT_VINCULUM			
				case RIGHT_VINCULUM:
					return ER_OK;
					break;
		// 6) <st-list>   -> [INT/DOUBLE/SRING] ID [ SEMICOLON / ASSIGN <assign>] <st-list>
				case INT:
				case DOUBLE:
				case STRING:
					if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID) {
                            // nelze definovat lokalni promennou s teckou v nazvu
                            if (strstr(token_data, ".") != NULL)
                                return ER_SEM;

							if(st_find(local_tabulka,token_data) != NULL)
								return ER_SEM;

							local_item = item = st_add(local_tabulka, token_data);

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

						if ( (token = get_next_token(&token_data)) != ER_LEX && token == SEMICOLON){
							item->elem_type = ST_ELEMTYPE_VAR;
							if( (result = statement_list()) != ER_OK){
								return result;
							}
							else{
								return ER_OK;
							}
						}
						else if(token == ASSIGN){
							if( (result = assign()) != ER_OK ){
								return result;
							}
							else{
								add_instr(IN_MOVSTACK,NULL,NULL,(void*) local_item);
								if( (result = statement_list()) != ER_OK){
									return result;
								}
								else{
									return ER_OK;
								}
							}
						}
                    }
					if(token == ER_LEX)
						return ER_LEX;

					return ER_SYNTAX;
					break;	
		 // 7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list>
				case ID:
					if ( (result = func_var()) != ER_OK){
						return result;
					}
					if ( (result = statement_list()) != ER_OK)
						return result;
					else return ER_OK;
					break;
			}
			return ER_SYNTAX;
	}
	return ER_SYNTAX;	
}

// <func-params>//function DECLARATION + INIT
// 		1) <func-params> -> RIGHT_BRACKET
// 		2) <func-params> -> [INT/DOUBLE/SRING] ID <func-params-list>
int func_params(){
	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	int result;
	int temp_token;

	switch(token){
// 1) <func-params> -> RIGHT_BRACKET		
		case RIGHT_BRACKET:
			return ER_OK;
			break;
// 2) <func-params> -> [INT/DOUBLE/SRING] ID <func-params-list>			
		case INT:
		case DOUBLE:
		case STRING:
			temp_token = token;
			if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID){

				//pridavame parametry jenom pri prvnim pruchodu
				if(pruchod == 0){
					item = st_add(current_function->local_table,token_data);	
					current_function->first_param = item;
                    current_function->last_param = item;
					item->elem_type = ST_ELEMTYPE_PARAM;
					item->next_param = NULL;
					item->prev_param = NULL;
					item->declared = 1;
					switch(temp_token){
						case INT:
							item->data_type =  ST_DATATYPE_INT;
							break;
						case DOUBLE:
							item->data_type =  ST_DATATYPE_DOUBLE;
							break;
						case STRING:
							item->data_type =  ST_DATATYPE_STRING;
							break;
					}
				}

				if ( (result = func_params_list()) != ER_OK)
					return result;
				else return ER_OK;
			}
			if(token == ER_LEX)
				return ER_LEX;
			return ER_SYNTAX;
			break;
	}
	return ER_SYNTAX;
}

// <func-params-list>
// 		1) <func-params-list> -> RIGHT_BRACKET
// 		2) <func-params-list> -> COMMA [INT/DOUBLE/SRING] ID <func-params-list>
int func_params_list(){
	int result;
	int temp_token;

	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	switch(token){
		case RIGHT_BRACKET:
			return ER_OK;
			break;
		case COMMA:
			if ( (token = get_next_token(&token_data)) != ER_LEX && (token == INT || token == DOUBLE || token == STRING)){
				temp_token = token;
				if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID){

					if(pruchod == 0){
						if(st_find(current_function->local_table,token_data) != NULL){
							return ER_SEM;// error number 3 redefenition of defined variable
						}
						item = st_add(current_function->local_table,token_data);	
						item->elem_type = ST_ELEMTYPE_PARAM;

                        current_function->last_param->next_param = item;
						item->next_param = NULL;
                        item->prev_param = current_function->last_param;
                        current_function->last_param = item;
						item->declared = 1;
						switch(temp_token){
							case INT:
								item->data_type =  ST_DATATYPE_INT;
								break;
							case DOUBLE:
								item->data_type =  ST_DATATYPE_DOUBLE;
								break;
							case STRING:
								item->data_type =  ST_DATATYPE_STRING;
								break;
						}
					}


					if ( (result = func_params_list()) != ER_OK)
						return result;
					else return ER_OK;
				}
			}	
			if(token == ER_LEX)
				return ER_LEX;
			return ER_SYNTAX;	
			break;
	}
	return ER_SYNTAX;
}

// <func_var>
// 		1) LEFT_BRACKET <func-args> SEMICOLON //its function call
// 		2) ASSIGN <assign> //its inicialization of var
int func_var(){
	int result;
	id = token_data;
	symtab_elem_t * local_item;

	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	switch(token){
// 1) LEFT_BRACKET <func-args> SEMICOLON //it's function call	
		case LEFT_BRACKET:
			if( (item = st_find_global(tabulka, id, current_class->id)) == NULL) //if function not in symtab
				return ER_SEM; //error type 3 not declarated function

			called_function = item;

            if (called_function->elem_type == ST_ELEMTYPE_FUN) { // if normal function
			    if ( (result = func_args()) != ER_OK)
			    	return result;
            }
            else if (called_function->elem_type == ST_ELEMTYPE_BUILTIN) { // if builtin function
                if (strcmp(called_function->id, "ifj16.print") == 0) {
                    if ( (result = func_args_print()) != ER_OK)
                        return result;
                }
                else {
			        if ( (result = func_args()) != ER_OK)
    			    	return result;
                }
            }
            else { // if not a function
                free(id);
                return ER_SEM;
            }

			add_instr(IN_CALL, NULL, NULL, (void *) called_function); // instruction for FUNCTION CALL


			if ( (token = get_next_token(&token_data)) != ER_LEX && token == SEMICOLON)
				return ER_OK;
			if(token == ER_LEX)
				return ER_LEX;
			return ER_SYNTAX;	
			break;
// 2) ASSIGN <assign> //its inicialization of var			
		case ASSIGN:
			if( (local_item = item = st_find(local_tabulka, id)) == NULL){
				if( (local_item = item = st_find(tabulka, id)) == NULL){ 
					if( (local_item = item = st_find(tabulka, id = str_conc(current_class->id, id))) == NULL){ 
						free(id);
						return ER_SEM; //error type 3 not declarated var
					}
					else{
						free(id);
					}	
				}	
			}
			if ( (result = assign()) != ER_OK)
				return result;
			else{ 
				add_instr(IN_MOVSTACK,NULL,NULL,(void*) local_item);
				return ER_OK;
			}
			break;
	}
	return ER_SYNTAX;
}

// <return-args>
// 		1) <return-args> -> SEMICOLON (ONLY if we in VOID function)
// 		4) <return-args> -> <math-expr> SEMICOLON
int return_args(){
	int result;
	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	switch(token){
// 1) <return-args> -> SEMICOLON (ONLY if we in VOID function)		
		case SEMICOLON:

			if(current_function->data_type != ST_DATATYPE_VOID){
				return ER_RUN_INIT ; //return nothing from non-VOID function //FIXME
			} 
			add_instr(IN_RETURN, NULL, NULL, NULL);
			return ER_OK;
			break;
		default:

			if(current_function->data_type == ST_DATATYPE_VOID){
				return 8; // return smth in void function
			}

			return_token(token, token_data);
			if( (result = math_expr(&type)) == ER_OK){

				if(current_function->data_type == ST_DATATYPE_DOUBLE && type == ST_DATATYPE_INT){
					add_instr(IN_CONV, NULL, NULL, NULL);
					type = ST_DATATYPE_DOUBLE;
				}
				if(current_function->data_type != (unsigned int)type){
					return ER_SEM_TYPES; //return wrong type
				}
				else{
					add_instr(IN_RETURN, NULL, NULL, NULL);
				}
				if(token == SEMICOLON){
					return ER_OK;
				}
				else{ 
					return ER_SYNTAX;
				}
			}
			else{
				return result;
			}
	}
	return ER_SYNTAX;
}


// <assign>	
// 		2) <assign>	   -> ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) SEMICOLON 
// 		3) <assign>	   -> ID [SEMICOLON/ <math-expr>]
// 		4) <assign>	   -> <math-expr> SEMICOLON
int assign(){
	int result;
    int temp_token;
    char *temp_token_data;
    symtab_elem_t * temp_elem;


	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	switch(token){
		case ID:
            temp_token = token;
            temp_token_data = strdup(token_data);

	        if( (temp_elem = st_find(local_tabulka,temp_token_data)) == NULL){
                if( (temp_elem  = st_find_global(tabulka,temp_token_data, current_class->id)) == NULL){
                    free(temp_token_data);
                    return ER_SEM; //error type 3 not declarated var
                } 
	        } 

			if ( (token = get_next_token(&token_data)) == ER_LEX )
				return ER_LEX;
			switch(token){
// 2) <assign>	   -> ID LEFT_BRACKET <func-args> SEMICOLON //function call			
				case LEFT_BRACKET: 
					if(temp_elem->elem_type != ST_ELEMTYPE_FUN && temp_elem->elem_type != ST_ELEMTYPE_BUILTIN){
						return ER_SEM;
					}

					called_function = temp_elem;
                    
					if ( (result = func_args()) != ER_OK)
						return result;

			        add_instr(IN_CALL, NULL, NULL, (void *) temp_elem);

					if ( (token = get_next_token(&token_data)) != ER_LEX && token == SEMICOLON)
						return ER_OK;

					if(token == ER_LEX)
						return ER_LEX;

					return ER_SYNTAX;	
					break;
// 3) <assign>	   -> ID SEMICOLON					
				case SEMICOLON:
					if(!(item->declared) ||  !(temp_elem->declared) || !(temp_elem->initialized) )
						return ER_SEM; //error number 3

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
							return ER_SEM;
					}
						
					return ER_OK;
					break;
// 3) <assign>	   -> ID ...-> <math-expr>					
                default:
                    return_token(token, token_data);
                    return_token(temp_token, temp_token_data);

                    result = math_expr(&type);

                    if(result == ER_OK){
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
                        			return ER_SEM; 
                        	}
							item->initialized = 1;
							return ER_OK;
						}
						else
							return ER_SEM; //error type 3
                    }
                    return result;
            }
        default:
            return_token(token, token_data);

            result = math_expr(&type);

            if (result != ER_OK)
                return result;

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
            			return ER_SEM; 
            	}

					item->initialized = 1;
			}
			else
				return ER_SEM; //error type 3
            

			return result;
	}
	return ER_SYNTAX;
}

// <func-args>//function CALL
		// 1) <func-args> -> RIGHT_BRACKET
		// 2) <func-args> -> [ID/INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] <func-args-list>
int func_args(){
	symtab_elem_t * temp_item;
	current_param = called_function->first_param;

	int result;

	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	switch(token){
		case RIGHT_BRACKET:
			return ER_OK;
			break;
		case ID:

			if( (temp_item = st_find(local_tabulka,token_data)) == NULL) {
				if( (temp_item = st_find_global(tabulka, token_data, current_class->id)) == NULL){
                    return ER_SEM; //error type 3 not declarated var
                }
            }

			add_instr(IN_TAB_PUSH,(void *)temp_item,NULL,NULL);	 //push function argument(ID) to stack

			//argument's type check	
			if(current_param == NULL)
                return ER_SEM_TYPES;

            if (temp_item->data_type != current_param->data_type) {
                if (temp_item->data_type == ST_DATATYPE_INT &&
                    current_param->data_type == ST_DATATYPE_DOUBLE)
                    add_instr(IN_CONV, NULL, NULL, NULL);
                else
    				return ER_SEM_TYPES;
			}

			if ( (result = func_args_list()) != ER_OK)
				return result;
			else return ER_OK;
			break;
		case INT_LITERAL:
		case DOUBLE_LITERAL:
		case STRING_LITERAL:

			//argument's type check	
			switch(token){
				case INT_LITERAL:
					if(current_param == NULL || (ST_DATATYPE_INT != current_param->data_type)){
						return ER_SEM_TYPES; //wrong parameter's type or number
					}

        			add_instr(IN_VAL_PUSH, (void *)(unsigned long)int_from_token(token_data),NULL,NULL); //push function argument(const) to stack
					break;
				case DOUBLE_LITERAL:
					if(current_param == NULL || (ST_DATATYPE_DOUBLE != current_param->data_type)){
						return ER_SEM_TYPES; //wrong parameter's type or number
					}

        			add_instr(IN_VAL_PUSH, (void *)double_from_token(token_data),NULL,NULL); //push function argument(const) to stack
					break;
				case STRING_LITERAL:
					if(current_param == NULL || (ST_DATATYPE_STRING != current_param->data_type)){
						return ER_SEM_TYPES; //wrong parameter's type or number
					}

        			add_instr(IN_VAL_PUSH, (void *)string_from_token(token_data),NULL,NULL); //push function argument(const) to stack
					break;
			}

			if ( (result = func_args_list()) != ER_OK)
				return result;
			else return ER_OK;
			break;
	}
	return ER_SYNTAX;
}

// <func-args-print>//function CALL
		// 1) <func-args> -> STRING_CONCAT RIGHT_BRACKET
int func_args_print(){
	return string_concat();
}

// <func-args-list>
// 		1) <func-args-list> -> RIGHT_BRACKET
// 		2) <func-args-list> -> COMMA [ID/INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] <func-args-list>
int func_args_list(){
	symtab_elem_t * temp_item;
	int result;
	current_param = current_param->next_param;

	if ( (token = get_next_token(&token_data)) == ER_LEX )
		return ER_LEX;
	switch(token){
		case RIGHT_BRACKET:
			return ER_OK;
			break;
		case COMMA:
			if ( (token = get_next_token(&token_data)) == ER_LEX )
				return ER_LEX;
			switch(token){
					case RIGHT_BRACKET:
						return ER_OK;
						break;
					case ID:
						if( (temp_item = st_find(local_tabulka,token_data)) == NULL) {
							if( (temp_item = st_find_global(tabulka, token_data, current_class->id)) == NULL){
                                return ER_SEM; //error type 3 not declarated var
                            }
                        }

						if (current_param == NULL) {
							return ER_SEM_TYPES;
						}		

						add_instr(IN_TAB_PUSH,(void *)temp_item,NULL,NULL);	 //push function argument(ID) to stack

                        if (temp_item->data_type != current_param->data_type) {
                            if (temp_item->data_type == ST_DATATYPE_INT &&
                                current_param->data_type == ST_DATATYPE_DOUBLE)
                                add_instr(IN_CONV, NULL, NULL, NULL);
                            else
                                return ER_SEM_TYPES;
                        }

						if ( (result = func_args_list()) != ER_OK)
							return result;
						else return ER_OK;
						break;
					case INT_LITERAL:
					case DOUBLE_LITERAL:
					case STRING_LITERAL:

						//argument's type check	
						switch(token){
							case INT_LITERAL:
								if(current_param == NULL) {
									return ER_SEM_TYPES; //wrong parameter's number
								}

                    			add_instr(IN_VAL_PUSH, (void *)(unsigned long)int_from_token(token_data),NULL,NULL); //push function argument(const) to stack

								if(ST_DATATYPE_INT != current_param->data_type){
                                    if (current_param->data_type == ST_DATATYPE_DOUBLE)
                                        add_instr(IN_CONV, NULL, NULL, NULL);
                                    else
                                        return ER_SEM_TYPES;
                                }
								break;
							case DOUBLE_LITERAL:
								if(current_param == NULL || (ST_DATATYPE_DOUBLE != current_param->data_type)){
									return ER_SEM_TYPES; //wrong parameter's type  or number
								}

                    			add_instr(IN_VAL_PUSH, (void *)double_from_token(token_data),NULL,NULL); //push function argument(const) to stack
								break;
							case STRING_LITERAL:
								if(current_param == NULL || (ST_DATATYPE_STRING != current_param->data_type)){
									return ER_SEM_TYPES; //wrong parameter's type  or number
								}

                    			add_instr(IN_VAL_PUSH, (void *)string_from_token(token_data),NULL,NULL); //push function argument(const) to stack
								break;
						}

						if ( (result = func_args_list()) != ER_OK)
							return result;
						else return ER_OK;
						break;
			}
			return ER_SYNTAX;
	}
	return ER_SYNTAX;
}

// <class-dec>
// 		1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID \n
//               [SEMICOLON/ ASSIGN <math-expr>/LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>] <class-dec>
// 		2) <class-dec> -> RIGHT_VINCULUM
// 		3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>
int class_dec(){
	int result;
	int prev_token;
	symtab_elem_t * find;
	char * temp_string;

	if(pruchod == 0){
		if ( (token = get_next_token(&token_data)) == ER_LEX )
			return ER_LEX;
		switch(token){
			case STATIC:
				if ( (token = get_next_token(&token_data)) == ER_LEX )
					return ER_LEX;
				switch(token){				
					case INT:
					case DOUBLE:
					case STRING:
						prev_token=token;
						if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID){

							if( (find = st_find(tabulka,temp_string = str_conc(current_class->id,token_data))) != NULL){
								if(find->declared == 1 || find->initialized == 1){
									free(temp_string);
									return ER_SEM; //redeclaration of existing symbol
								}
								else{
									free(temp_string);
									find->declared = 1; //(must NOT happen) if ID exist in global table but not declared and not initialized 
								}
							}
							else{
								item = st_add(tabulka,temp_string);
								item->declared = 1;
                                item->is_global = true;
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

							if ( (token = get_next_token(&token_data)) == ER_LEX )
								return ER_LEX;
							switch(token){
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID SEMICOLON <class-dec>							
								case SEMICOLON:
									
									item->initialized = 0;
									item->elem_type = ST_ELEMTYPE_VAR;
                                    item->is_global = true;

									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID  ASSIGN <math-expr> <class-dec>								
								case ASSIGN:
									item->elem_type = ST_ELEMTYPE_VAR;

									if ( (result = math_expr(&type)) != ER_OK)
										return result;
                                    
                                    add_instr(IN_MOVSTACK, NULL, NULL, (void *) item);

									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>							
								case LEFT_BRACKET:

									current_function = item;
									current_function->elem_type = ST_ELEMTYPE_FUN;
                                    current_function->is_global = true;
                                    st_init(&(current_function->local_table));
                                    local_tabulka = current_function->local_table;

									if ( (result = func_params()) != ER_OK)
										return result;
									if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){

										current_function->initialized = 1;

										if ( (result = statement_list()) != ER_OK)
											return result;
										if ( (result = class_dec()) != ER_OK)
											return result;
										else return ER_OK;
									}
									if(token == ER_LEX)
										return ER_LEX;
									return ER_SYNTAX;	
									break;
							}
							return ER_SYNTAX;
						}
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;	
						break;
	// 3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params> LEFT_VINCULUM <st-list> <class-dec>					
					case VOID:
						if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID){

							if( (find = st_find(tabulka,temp_string = str_conc(current_class->id,token_data))) != NULL){
								if(find->declared == 1 || find->initialized == 1){
									free(temp_string);
									return ER_SEM; //redeclaration of existing symbol
								}
								else{
									free(temp_string);
									find->declared = 1;
								}
							}
							else{

								current_function = st_add(tabulka,temp_string);
								current_function->elem_type = ST_ELEMTYPE_FUN;
								current_function->data_type = ST_DATATYPE_VOID;
								current_function->declared =  1;
                                st_init(&(current_function->local_table));
                                local_tabulka = current_function->local_table;
							}
							id = token_data;
							if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET){
								if ( (result = func_params()) != ER_OK)
									return result;
								if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){

									current_function->initialized = 1;

									if ( (result = statement_list()) != ER_OK)
										return result;
									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
								}
							}
						}
						else if(token == RUN){
							if(strcmp(current_class->id, "Main") == 0)
								run_counter++;

							if( (find = st_find(tabulka,temp_string = str_conc(current_class->id,token_data))) != NULL){
								if(find->declared == 1 || find->initialized == 1){
									free(temp_string);
									return ER_SEM; //redeclaration of existing symbol
								}
								else{
									free(temp_string);
									find->declared = 1;
									current_function = find;
									local_tabulka = current_function->local_table;
								}
							}
							else{
								current_function = st_add(tabulka,temp_string);
								current_function->elem_type = ST_ELEMTYPE_FUN;
								current_function->data_type = ST_DATATYPE_VOID;
								current_function->declared =  1;
                                st_init(&(current_function->local_table));
                                local_tabulka = current_function->local_table;
							}
							id = token_data;
							if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET){
								if ( (token = get_next_token(&token_data)) != ER_LEX && token != RIGHT_BRACKET)
									return ER_SYNTAX;
								if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){

									current_function->initialized = 1;

									if ( (result = statement_list()) != ER_OK)
										return result;
									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
								}
							}
						}	
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;	
						break;		
				}
				return ER_SYNTAX;	
				break;
	// 2) <class-dec> -> RIGHT_VINCULUM			
			case RIGHT_VINCULUM:
				return ER_OK;
				break;
		}
		return ER_SYNTAX;
	}

	//pruchod 2
	else{
        local_tabulka = NULL;

		if ( (token = get_next_token(&token_data)) == ER_LEX )
			return ER_LEX;
		switch(token){
			case STATIC:
				if ( (token = get_next_token(&token_data)) == ER_LEX )
					return ER_LEX;
				switch(token){				
					case INT:
					case DOUBLE:
					case STRING:
						prev_token=token;
						if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID){

							item = st_find(tabulka,temp_string = str_conc(current_class->id,token_data));
							free(temp_string);

							if ( (token = get_next_token(&token_data)) == ER_LEX )
								return ER_LEX;
							switch(token){
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID SEMICOLON <class-dec>							
								case SEMICOLON:
									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID  ASSIGN <math-expr> <class-dec>								
								case ASSIGN:
									if ( (result = math_expr(&type)) != ER_OK)
										return result;

									add_instr(IN_MOVSTACK,NULL,NULL,(void *)item);

									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
									break;
	// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>							
								case LEFT_BRACKET:
									if ( (result = func_params()) != ER_OK)
										return result;
                                    current_function = item;
                                    local_tabulka = current_function->local_table;

                                    set_function_beginning(&(current_function->first_instr));

									if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){
										if ( (result = statement_list()) != ER_OK)
											return result;
										if ( (result = class_dec()) != ER_OK)
											return result;
										else return ER_OK;
									}
									if(token == ER_LEX)
										return ER_LEX;
									return ER_SYNTAX;	
									break;
							}
							return ER_SYNTAX;
						}
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;	
						break;
	// 3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>					
					case VOID:
						if ( (token = get_next_token(&token_data)) != ER_LEX && token == ID){
							current_function = st_find(tabulka,temp_string = str_conc(current_class->id,token_data));

                            if (current_function == NULL)
                                return ER_INTERN;

                            local_tabulka = current_function->local_table;
							free(temp_string);

							if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET){
								if ( (result = func_params()) != ER_OK)
									return result;

                                set_function_beginning(&(current_function->first_instr));

								if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){
									if ( (result = statement_list()) != ER_OK)
										return result;

									add_instr(IN_RETURN,NULL,NULL,NULL);

									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
								}
							}
						}
						else if(token == RUN){
							current_function = st_find(tabulka,temp_string = str_conc(current_class->id,token_data));
                            local_tabulka = current_function->local_table;
							free(temp_string);

							if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_BRACKET){
								if ( (token = get_next_token(&token_data)) != ER_LEX && token != RIGHT_BRACKET)
									return ER_SYNTAX;
								if ( (token = get_next_token(&token_data)) != ER_LEX && token == LEFT_VINCULUM){
									if ( (result = statement_list()) != ER_OK)
										return result;

									add_instr(IN_RETURN,NULL,NULL,NULL);

									if ( (result = class_dec()) != ER_OK)
										return result;
									else return ER_OK;
								}
							}
						}	
						if(token == ER_LEX)
							return ER_LEX;
						return ER_SYNTAX;	
						break;		
				}
				return ER_SYNTAX;	
				break;
	// 2) <class-dec> -> RIGHT_VINCULUM			
			case RIGHT_VINCULUM:
				return ER_OK;
				break;
		}
		return ER_SYNTAX;
	}	
}
