#ifndef ILIST.H
#define ILIST.H


typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;
    

typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;


void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr); 
void *listGetPointerLast(tListOfInstr *L);


#endif