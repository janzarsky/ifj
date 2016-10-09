#ifndef ILIST.H
#define ILIST.H

/*Definovat nase typy instrukce !!
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr instruction;
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
void listGoto(tListOfInstr *L, tListItem *gotoInstr); 
void *listGetPointerLast(tListOfInstr *L);


#endif