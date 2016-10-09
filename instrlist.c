#include "instrlist.h"

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
  else return &(L->active->Instruction);

}

void listGoto(tListOfInstr *L, tListItem *gotoInstr)
{
  L->active =  gotoInstr;
}