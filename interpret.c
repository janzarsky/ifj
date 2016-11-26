/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "string.h"
#include "symtab.h"
#include "instrlist.h"
#include "interpret.h"
#include "instrlist.c"



int interpret(symtab_t *T, tListOfInstr *L)
{
  inter_stack S;
  S.top = NULL;
  listFirst(L); // nastav aktivni prvni instrukci
  tInstr *I;

    inter_stack_item first;
    inter_stack_item second;
    inter_stack_item third;
  while (1)
  {
      I = listGetData(L); // ziskej instrukci


      switch (I->instType)
    {
        case TAB_PUSH:

            push_tab((((symtab_elem_t *)(I->addr1))->value), &S);

        break;

        case VAL_PUSH:

            push_val(I->addr1, &S);

        break;

        case IN_ADD:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.ival = first.value.union_value.ival + second.value.union_value.ival;
            push_tab(third.value.union_value, &S);
        break;


        case IN_SUB:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.ival = first.value.union_value.ival - second.value.union_value.ival;
            push_tab(third.value.union_value, &S);
        break;

        case IN_MUL:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.ival = first.value.union_value.ival * second.value.union_value.ival;
            push_tab(third.value.union_value, &S);
        break;

        case IN_DIV:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.ival = first.value.union_value.ival / second.value.union_value.ival;
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_ADD:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = first.value.union_value.dval + second.value.union_value.dval;
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_SUB:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = first.value.union_value.dval - second.value.union_value.dval;
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_MUL:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = first.value.union_value.dval * second.value.union_value.dval;
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_DIV:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = first.value.union_value.dval / second.value.union_value.dval;
            push_tab(third.value.union_value, &S);
        break;

        case IN_CONV:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            first.value.union_value.dval = (double)first.value.union_value.ival;

            push_tab(first.value.union_value, &S);

        break;

        case IN_SWAP:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            push_tab(first.value.union_value, &S);
            push_tab(second.value.union_value, &S);

        break;

        case IN_CONCAT:

           stack_inter_Top(&(first.value), &S);
           stack_inter_Pop(&S);

           stack_inter_Top(&(second.value), &S);
           stack_inter_Pop(&S);


           strcpy(third.value.union_value.strval, first.value.union_value.strval);
           strcat(third.value.union_value.strval, second.value.union_value.strval);

           push_tab(third.value.union_value, &S);

        break;

        case IN_LESS:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.ival < second.value.union_value.ival) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_GREAT:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.ival > second.value.union_value.ival) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_LESS_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.ival <= second.value.union_value.ival) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_GREAT_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.ival >= second.value.union_value.ival) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.ival == second.value.union_value.ival) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_N_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.ival != second.value.union_value.ival) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_F_LESS:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.dval < second.value.union_value.dval) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_F_GREAT:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.dval > second.value.union_value.dval) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_F_LESS_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.dval <= second.value.union_value.dval) third.value.union_value.dval = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_F_GREAT_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.dval >= second.value.union_value.dval) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_F_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.dval == second.value.union_value.dval) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;

        case IN_F_N_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);

            push_tab(second.value.union_value, &S);

            if (first.value.union_value.dval != second.value.union_value.dval) third.value.union_value.ival = 1;

            else third.value.union_value.ival = 0;

            push_tab(third.value.union_value, &S);

        break;




    }
 }
}

void push_tab(st_value_t val, inter_stack *S) {

    inter_stack_item *temp = (inter_stack_item *)malloc(sizeof(inter_stack_item));
    if (temp == NULL) return;

    temp->next = S->top;
    temp->value.union_value = val;

    S->top = temp;
}

void push_val(void *val, inter_stack *S) {

    inter_stack_item *temp = (inter_stack_item *)malloc(sizeof(inter_stack_item));
    if (temp == NULL) return;

    temp->next = S->top;
    temp->value.vval = val;

    S->top = temp;
}

void stack_inter_Top(inter_value *val, inter_stack *S){

    *val = S->top->value;
}

void stack_inter_Pop(inter_stack *S){

    inter_stack_item *temp = (*S).top;
    (*S).top = (*S).top->next;
    free(temp);

}*/

