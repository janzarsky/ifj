#include <stdio.h>
#include <stdlib.h>
#include "instrlist.h"

tListOfInstr *instr_list;

void listInit(tListOfInstr *L){

	L->first = NULL;
	L->active = NULL;
	L->last = NULL;
	
}

void listFree(tListOfInstr *L){

	for(L->active = L->first;L->first != NULL;L->first = L->active){
			L->active = L->first->nextItem;		//zachovavame pointer na nastupny prvek
			free(L->first);
		}
	L->last = NULL;

}

void listInsertLast(tListOfInstr *L, tInstr I){

	tListItem * item;
	item = malloc(sizeof(tListItem));

	if(item == NULL){
		printf("Allocation error");
		exit(99);
	}

	item->instruction = I;
	item->nextItem = NULL;

	if(L->first != NULL){
	    L->last->nextItem = item;
	    L->last = item;
	}    
	else{
		L->first = L->last = item;
	}

}

#ifdef DEBUG
void print_instr(tInstr *instr) {
    printf("%p ", (void *)instr);

    switch (instr->instType) {
        case IN_ADD:
            printf("ADD"); break;
        case IN_SUB:
            printf("SUB"); break;
        case IN_MUL:
            printf("MUL"); break;
        case IN_DIV:
            printf("DIV"); break;
        case IN_F_ADD:
            printf("FADD"); break;
        case IN_F_SUB:
            printf("FSUB"); break;
        case IN_F_MUL:
            printf("FMUL"); break;
        case IN_F_DIV:
            printf("FDIV"); break;
        case IN_VAL_PUSH:
            printf("VAL_PUSH"); break;
        case IN_TAB_PUSH:
            printf("TAB_PUSH"); break;
        case IN_CONV:
            printf("CONV"); break;
        case IN_SWAP:
            printf("SWAP"); break;
        case IN_CONCAT:
            printf("CONCAT"); break;
        case IN_LESS:
            printf("LESS"); break;
        case IN_GREAT:
            printf("GREAT"); break;
        case IN_LESS_EQ:
            printf("LES_EQ"); break;
        case IN_GREAT_EQ:
            printf("GREAT_EQ"); break;
        case IN_EQ:
            printf("EQ"); break;
        case IN_N_EQ:
            printf("N_EQ"); break;
        case IN_F_LESS:
            printf("F_LESS"); break;
        case IN_F_GREAT:
            printf("F_GREAT"); break;
        case IN_F_LESS_EQ:
            printf("F_LES_EQ"); break;
        case IN_F_GREAT_EQ:
            printf("F_GREAT_EQ"); break;
        case IN_F_EQ:
            printf("F_EQ"); break;
        case IN_F_N_EQ:
            printf("F_N_EQ"); break;
        case IN_GOTO:
            printf("GOTO"); break;
        case IN_LABEL:
            printf("LABEL"); break;
        case IN_MOVSTACK:
            printf("MOVSTACK"); break;
        case IN_RETURN:
            printf("RETURN"); break;
        case IN_CALL:
            printf("CALL"); break;
        default:
            printf("%d", instr->instType);
    }

    printf(" %p %p %p, ", instr->addr1, instr->addr2, instr->addr3);
}

void print_instr_list() {
    tInstr *instr;

    listFirst(instr_list);

    while (instr_list->active != NULL) {
        instr = listGetData(instr_list);

        print_instr(instr);
        printf("\n");

        listNext(instr_list);
    }
}
#endif

void add_instr(int type, void * ptr1, void * ptr2, void * ptr3) {
    tInstr instr = { type, ptr1, ptr2, ptr3 };

    listInsertLast(instr_list, instr);

#ifdef DEBUG
    printf("instr: ");
    print_instr(&instr);
#endif
}

void set_function_beginning(tListItem **item) {
    add_instr(IN_LABEL, NULL, NULL, NULL);
    *item = instr_list->last;
}

void listFirst(tListOfInstr *L){

	 L->active = L->first;

}

void listNext(tListOfInstr *L){

	if (L->active != NULL)
  		L->active = L->active->nextItem;

}

tInstr *listGetData(tListOfInstr *L)
{

  if (L->active == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    exit(99);
  }
  else return &(L->active->instruction);

}

void listGoto(tListOfInstr *L, tListItem *gotoInstr)
{
  L->active =  gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L)
{
	return (void *)L->last;
}
