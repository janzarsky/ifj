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

#include "instrlist.h"

tListOfInstr *instr_list;

void listInit(tListOfInstr *L){
	L->first = NULL;
	L->active = NULL;
	L->last = NULL;
}

void listFree(tListOfInstr *L){
    L->active = L->first;

	while (L->first != NULL) {
        L->active = L->first->nextItem;
        free(L->first);
        L->first = L->active;
    }

	L->last = NULL;
}

void listInsertLast(tListOfInstr *L, tInstr I){

	tListItem * item;
	item = malloc(sizeof(tListItem));

	if (item == NULL)
        return;

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

void add_instr(int type, void * ptr1, void * ptr2, void * ptr3) {
    tInstr instr = { type, ptr1, ptr2, ptr3 };

    listInsertLast(instr_list, instr);
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

tInstr *listGetData(tListOfInstr *L) {
  if (L->active == NULL)
      return NULL;
      
  return &(L->active->instruction);
}

void listGoto(tListOfInstr *L, tListItem *gotoInstr) {
  L->active =  gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L) {
	return (void *)L->last;
}
