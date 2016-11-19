#include <stdio.h>
#include "str.h"
#include "symtab.h"
#include "instrlist.h"

typedef struct stack_item {
    st_value_t hodnota;
    struct stack_item *next;
   } stack_item_t;

   typedef struct stack{
    stack_item_t *top;
   } stack_t;

void push(st_value_t val);


int interpret(symtab_t *T, tListOfInstr *L)
{
  stack_t stack = { NULL };
  listFirst(L); // nastav aktivni prvni instrukci
  tInstr *I;
  while (1)
  {
      I = listGetData(L); // ziskej instrukci

      switch (I->instType)
    {
        case: IN_ADD
        (st_value_t*)I->addr1 = (st_value_t*)I->addr2 + (st_value_t*)I->addr3;
        break;

        case: IN_SUB
        (st_value_t*)I->addr1 = (st_value_t*)I->addr2 - (st_value_t*)I->addr3;
        break;

        case: IN_MUL
        (st_value_t*)I->addr1 = (st_value_t*)I->addr2 * (st_value_t*)I->addr3;
        break;

        case: IN_DIV
        (st_value_t*)I->addr1 = (st_value_t*)I->addr2 / (st_value_t*)I->addr3;
        break;

        case: IN_PUSH
         push((st_value_t*)I->addr1);
        break;

        case: IN_MAX
        if ((st_value_t*)I->addr2 > (st_value_t*)I->addr3)
            (st_value_t*)I->addr1 = (st_value_t*)I->addr2;



    }
 }
}

void push(st_value_t val) {

    stack_item_t *temp = malloc(sizeof(stack_item_t));
    if (temp == NULL) return;

    temp->next = stack.top;
    temp->hodnota = val;

    stack.top = temp;
}
