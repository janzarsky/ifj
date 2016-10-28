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
int statement[] = {CLASS, MAIN, LEFT_VINCULUM, STATIC, VOID, RUN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM,END_STATEMENT}; 

		// 2) <prog>      -> CLASS ID LEFT_VINCULUM <st-list> // declaration of clas
// int statement[] = {CLASS, ID, LEFT_VINCULUM,END_STATEMENT}; 

		// 3) <prog>      -> STATIC <func-dec>	//declaration and initialization of function
// int statement[] = {STATIC,END_STATEMENT}; 



int statement_list();
int function_declaration();
int program();
int token;

int get_next_token(){
	static int number=0;
	if(statement[number] != END_STATEMENT)
		return statement[number++];
	else
		return END_STATEMENT;
}

int program(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case CLASS:
			if ( (token = get_next_token()) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// <prog> 	   -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
				case MAIN:
						if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM)
							 if ( (token = get_next_token()) != LEX_ERROR && token == STATIC)
							 	if ( (token = get_next_token()) != LEX_ERROR && token == VOID)
							 		if ( (token = get_next_token()) != LEX_ERROR && token == RUN)
									 	if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET)
										 	if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
											 	if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
											 		if( (result = statement_list()) == SYNTAX_OK){
													 	if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_VINCULUM)
														 	if ( (token = get_next_token()) != LEX_ERROR && token == END_STATEMENT)
														 		return SYNTAX_OK;
											 		}
											 		else return result;
										 		}
						return SYNTAX_ERROR;
						break;	
// <prog>      -> CLASS ID LEFT_VINCULUM <class-dec> // declaration of class						
				case ID: 
						if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = statement_list()) == SYNTAX_OK){
								if ( (token = get_next_token()) != LEX_ERROR  && token == END_STATEMENT)
									return SYNTAX_OK;
					 		}
					 		else return result;
						}
						return SYNTAX_ERROR;
						break;
				default: return SYNTAX_ERROR;
			}
// <prog>      -> STATIC <func-dec>			
		case STATIC:
			if( (result = function_declaration()) == SYNTAX_OK){
				if ( ((token = get_next_token()) != LEX_ERROR) && token == END_STATEMENT ) 
					return SYNTAX_OK;
			}
			else return result;
			break;
		default: return SYNTAX_ERROR;
	}
	return SYNTAX_ERROR;
}


int statement_list(){
	
	return SYNTAX_OK;
}

int function_declaration(){
	return SYNTAX_OK;
}

int main(int argc, char ** argv){
	printf("**************************************************************************\n");
	printf("\t100 - SYNTAX_OK\n\t99  - SYNTAX_ERROR\n\t98  - LEX_ERROR\n");
	printf("**************************************************************************\n");

	 printf("result:%d\n", program());
	return SYNTAX_OK;
}