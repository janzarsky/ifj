#include "instrlist.h"
#include <stdio.h>

#define END_STATEMENT 66
#define SYNTAX_OK 100
#define SYNTAX_ERROR 99
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
// int statement[] = {CLASS, MAIN, LEFT_VINCULUM, STATIC, VOID, RUN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, RIGHT_VINCULUM, END_STATEMENT}; 

		// 2) <prog>      -> CLASS ID LEFT_VINCULUM <st-list> // declaration of clas
// int statement[] = {CLASS, ID, LEFT_VINCULUM,END_STATEMENT}; 

		// 3) <prog>      -> STATIC [INT/DOUBLE/SRING/VOID] ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
int statement[] = { STATIC, INT, ID , LEFT_BRACKET, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, END_STATEMENT}; 

int token;
int return_args();
int bool_expr();
int statement_list();
int func_var();
int program();
int equal();
int class_dec();
int func_args();
int func_args_list();


int get_next_token(){
	static int number=0;
	if(statement[number] != END_STATEMENT)
		return statement[number++];
	else
		return END_STATEMENT;
}

// 1) <prog> -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
// 2) <prog> -> CLASS ID LEFT_VINCULUM <class-dec> // declaration of clas
// 3) <prog> -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>	//declaration and initialization of function
int program(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case CLASS:
			if ( (token = get_next_token()) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// 1) <prog> 	   -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
				case MAIN:
						if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM)
							 if ( (token = get_next_token()) != LEX_ERROR && token == STATIC)
							 	if ( (token = get_next_token()) != LEX_ERROR && token == VOID)
							 		if ( (token = get_next_token()) != LEX_ERROR && token == RUN)
									 	if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET)
										 	if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
											 	if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
											 		if( (result = statement_list()) != SYNTAX_OK)
											 			return result;
											 		else
													 	if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_VINCULUM)
														 	if ( (token = get_next_token()) != LEX_ERROR && token == END_STATEMENT)
														 		return SYNTAX_OK;
												}
						if(token == LEX_ERROR)
							return LEX_ERROR;						
						return SYNTAX_ERROR;
						break;	
// 2) <prog>      -> CLASS ID LEFT_VINCULUM <class-dec> // declaration of class						
				case ID: 
						if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = class_dec()) != SYNTAX_OK)
					 			return result;
							if ( (token = get_next_token()) != LEX_ERROR  && token == END_STATEMENT)
								return SYNTAX_OK;
						}
						if(token == LEX_ERROR)
							return LEX_ERROR;
						return SYNTAX_ERROR;
						break;
			}
			return SYNTAX_ERROR;
// 3) <prog>      -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
		case STATIC:
			if ( (token = get_next_token()) != LEX_ERROR  && (token == VOID || token == INT || token == DOUBLE || token == STRING) )
				if ( (token = get_next_token()) != LEX_ERROR  && token == ID)
					if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_BRACKET){
						if( (result = func_args()) != SYNTAX_OK)
					 			return result;
					 	if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = statement_list()) != SYNTAX_OK)
					 			return result;
					 		//else return SYNTAX_OK;
					 		if ( (token = get_next_token()) != LEX_ERROR && token == END_STATEMENT)
								return SYNTAX_OK;
					 	}
					}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;	
	}
	return SYNTAX_ERROR;
}

/*st-list>//function body
		1) <st-list>   -> RETURN <return-args> <st-list> //return in function
		2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> <st-list>//cycle while
		3) <st-list>   -> IF LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list> //if-else statement
		4) <st-list>   -> RIGHT_VINCULUM	// end of sequence
		5) <st-list>   -> STATIC [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list> // ????????? can we declare static variables in func ?
		6) <st-list>   -> [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list>
		7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list> //it can be function call OR inicialization of var*/
int statement_list(){
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	int result;
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
			if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET){
				if ( (result = bool_expr()) != SYNTAX_OK)
					return result;
				if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
					if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
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
// 3) <st-list>   -> IF LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list>			
		case IF:
			if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET){
				if ( (result = bool_expr()) != SYNTAX_OK)
					return result;
				if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
					if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						if ( (token = get_next_token()) != LEX_ERROR && token == ELSE)
							if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
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
// 5) <st-list>   -> STATIC [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list>
		case STATIC:
			if ( (token = get_next_token()) != LEX_ERROR && (token == INT || token == DOUBLE || token == STRING) )
				if ( (token = get_next_token()) != LEX_ERROR && token == ID)
					if ( (token = get_next_token()) != LEX_ERROR && token == EQUAL){
						if ( (result = equal()) != SYNTAX_OK)
							return result;
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						else return SYNTAX_OK;
					}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
// 6) <st-list>   -> [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list>
		case INT:
		case DOUBLE:
		case STRING:
			if ( (token = get_next_token()) != LEX_ERROR && token == ID)
				if ( (token = get_next_token()) != LEX_ERROR && token == EQUAL){
					if ( (result = equal()) != SYNTAX_OK)
						return result;
					if ( (result = statement_list()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
				}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
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

// <func-args>
// 		1) <func-args> -> [INT/DOUBLE/SRING] ID <func-args-list>
// 		2) <func-args> -> RIGHT_BRACKET
int func_args(){
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	int result;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case INT:
		case DOUBLE:
		case STRING:
			if ( (token = get_next_token()) != LEX_ERROR && token == ID){
				if ( (result = func_args_list()) != SYNTAX_OK)
					return result;
				else return SYNTAX_OK;
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
	}
	return SYNTAX_ERROR;
}

int func_var(){
	return SYNTAX_OK;
}

int bool_expr(){
	return SYNTAX_OK;
}


int return_args(){
	return SYNTAX_OK;
}

int equal(){
	return SYNTAX_OK;
}

int class_dec(){
	return SYNTAX_OK;
}

int func_args_list(){
	return SYNTAX_OK;
}

int main(int argc, char ** argv){
	printf("**************************************************************************\n");
	printf("\t100 - SYNTAX_OK\n\t99  - SYNTAX_ERROR\n\t98  - LEX_ERROR\n");
	printf("**************************************************************************\n");

	 printf("result:%d\n", program());
	return SYNTAX_OK;
}