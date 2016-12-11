#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "string.h"
#include "symtab.h"
#include "instrlist.h"
#include "interpret.h"
#include "frames.h"
#include "error_codes.h"

int ifj_errno = 0;

void stack_inter_print(inter_stack *stack);
void alloc_double(double **dval_ptr);

int interpret(tListOfInstr *L)
{
  inter_stack S;
  S.top = NULL;
  bool_stack B;
  B.top = NULL;
  listFirst(L); // nastav aktivni prvni instrukci
  tInstr *I = NULL;
  bool hodnota;
  bool load_next = false;
  int result;
  char *str;

    inter_stack_item first;
    inter_stack_item second;
    inter_stack_item third;
    st_value_t value;

    I = listGetData(L);

  while (L->active != NULL)
  {
      if (load_next) {
          listNext(L);
      }

      I = listGetData(L); // ziskej instrukci

      load_next = true;

#ifdef DEBUG
      printf("INTERPRET: ");stack_inter_print(&S);printf("\n");
      printf("INTERPRET: instruction ");print_instr(I);printf("\n");
#endif

      switch (I->instType)
    {
        case IN_CALL:
            result = call_instr(L, &S, (symtab_elem_t *)I->addr3);

            if (result != ER_OK)
                return result;

            load_next = false;
        break;

        case IN_RETURN:
#ifdef DEBUG
            stack_inter_print(&S);
            printf("\n");
#endif

            result = return_instr(L);

#ifdef DEBUG
            stack_inter_print(&S);
            printf("\n");
#endif
            
            if (result == FR_NO_FRAMES) {
                return ER_OK;
            }

            load_next = false;
        break;

        case IN_INT_TO_STR:
            
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            str = malloc(20*sizeof(char));

            if (str == NULL)
                return ER_INTERN;

            sprintf(str, "%d", first.value.union_value.ival);

            third.value.union_value.strval = str;

            push_tab(third.value.union_value, &S);

        break;

        case IN_DBL_TO_STR:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            str = malloc(50*sizeof(char));

            if (str == NULL)
                return ER_INTERN;

            sprintf(str, "%g", *(first.value.union_value.dval));

            third.value.union_value.strval = str;

            push_tab(third.value.union_value, &S);

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

            third.value.union_value.ival = second.value.union_value.ival - first.value.union_value.ival;
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

            if (first.value.union_value.ival == 0)
                return ER_RUN_ZERO;

            third.value.union_value.ival = second.value.union_value.ival / first.value.union_value.ival;
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_ADD:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = malloc(sizeof(double)); 
            if (third.value.union_value.dval == NULL)
                return ER_INTERN;
            
            *(third.value.union_value.dval) = *(first.value.union_value.dval) + *(second.value.union_value.dval);

            push_tab(third.value.union_value, &S);
        break;

        case IN_F_SUB:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = malloc(sizeof(double)); 
            if (third.value.union_value.dval == NULL)
                return ER_INTERN;
            
            *(third.value.union_value.dval) = *(second.value.union_value.dval) - *(first.value.union_value.dval);
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_MUL:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = malloc(sizeof(double)); 
            if (third.value.union_value.dval == NULL)
                return ER_INTERN;
            
            *(third.value.union_value.dval) = *(first.value.union_value.dval) * *(second.value.union_value.dval);
            push_tab(third.value.union_value, &S);
        break;

        case IN_F_DIV:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);
            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(first.value.union_value.dval) == 0.0)
                return ER_RUN_ZERO;

            third.value.union_value.dval = malloc(sizeof(double)); 

            if (third.value.union_value.dval == NULL)
                return ER_INTERN;

            *(third.value.union_value.dval) = *(second.value.union_value.dval) / *(first.value.union_value.dval);
            push_tab(third.value.union_value, &S);
        break;
        
         case IN_TAB_PUSH:
            ifj_errno = ER_OK;

            value = get_value((symtab_elem_t *)(I->addr1));
            
            if (ifj_errno != ER_OK)
                return ifj_errno;

            push_tab(value, &S);
        break;

        case IN_VAL_PUSH:
            push_val(I->addr1, &S);
        break;

        case IN_CONV:
            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            third.value.union_value.dval = malloc(sizeof(double));

            if (third.value.union_value.dval == NULL)
                return ER_INTERN;
            
            *(third.value.union_value.dval) = (double) first.value.union_value.ival;

            push_tab(third.value.union_value, &S);
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

            int len = strlen(first.value.union_value.strval)
                + strlen(second.value.union_value.strval);
            
            third.value.union_value.strval = malloc((len+1)*sizeof(char));

            if (third.value.union_value.strval == NULL)
                return ER_INTERN;

            strcpy(third.value.union_value.strval, second.value.union_value.strval);
            strcat(third.value.union_value.strval, first.value.union_value.strval);

            //free(first.value.union_value.strval);
            //free(second.value.union_value.strval);

           push_tab(third.value.union_value, &S);

        break;

        case IN_LESS:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (second.value.union_value.ival < first.value.union_value.ival)
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_GREAT:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (second.value.union_value.ival > first.value.union_value.ival)
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_LESS_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (second.value.union_value.ival <= first.value.union_value.ival)
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_GREAT_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (second.value.union_value.ival >= first.value.union_value.ival)
                bool_Push(true, &B);
            else
                bool_Push(false, &B);
        break;

        case IN_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (second.value.union_value.ival == first.value.union_value.ival)
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_N_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (second.value.union_value.ival != first.value.union_value.ival)
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_F_LESS:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(second.value.union_value.dval) < *(first.value.union_value.dval))
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_F_GREAT:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(second.value.union_value.dval) > *(first.value.union_value.dval))
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_F_LESS_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(second.value.union_value.dval) <= *(first.value.union_value.dval))
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_F_GREAT_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(second.value.union_value.dval) >= *(first.value.union_value.dval))
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_F_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(second.value.union_value.dval) == *(first.value.union_value.dval))
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;

        case IN_F_N_EQ:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            stack_inter_Top(&(second.value), &S);
            stack_inter_Pop(&S);

            if (*(second.value.union_value.dval) != *(first.value.union_value.dval))
                bool_Push(true, &B);
            else
                bool_Push(false, &B);

        break;
        
        case IN_GOTO:
              listGoto(L, I->addr3);
              load_next = false;
        break;

        case IN_JMP_WHILE:

            bool_Top(&hodnota, &B);

            if (hodnota){
              listGoto(L, I->addr3);
              load_next = false;
            }
            else
                bool_Pop(&B);

        break;

        case IN_IFNGOTO:

            bool_Top(&hodnota, &B);
            bool_Pop(&B);

            if (hodnota == false){

              listGoto(L, I->addr3);
              load_next = false;
            }

        break;

         // rozsireni

        case IN_DEC:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            //first.value.union_value.ival = first.value.union_value.ival--;
            first.value.union_value.ival--;

            push_tab(first.value.union_value, &S);

        break;

        case IN_INC:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            //first.value.union_value.ival = first.value.union_value.ival++;
            first.value.union_value.ival++;

            push_tab(first.value.union_value, &S);

        break;

        case IN_F_DEC:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

           // first.value.union_value.dval = first.value.union_value.dval--;
           first.value.union_value.dval--;

            push_tab(first.value.union_value, &S);

        break;

        case IN_F_INC:

            stack_inter_Top(&(first.value), &S);
            stack_inter_Pop(&S);

            //first.value.union_value.dval = first.value.union_value.dval++;
            first.value.union_value.dval++;

            push_tab(first.value.union_value, &S);

        break;

        case IN_LABEL:
            
            
            
        break;

        case IN_MOVSTACK:

            stack_inter_Top(&(third.value), &S);
            stack_inter_Pop(&S);

            set_value(((symtab_elem_t *)I->addr3), &(third.value));

        break;
    }
 }
#ifdef DEBUG
      printf("INTERPRET: ");
      
      stack_inter_print(&S);

      printf("\n");
#endif

  return 0;
}

// FCE PRO PRACI SE ZASOBNIKEM

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
}

#ifdef DEBUG
void stack_inter_print(inter_stack *stack) {
    printf("inter stack (starting from top): ");

    inter_stack_item *temp = stack->top;

    while (temp != NULL) {
        printf("%p ", temp->value.vval);

        temp = temp->next;
    };
}
#endif

//FCE pro hodnoty bool
void bool_Pop(bool_stack *B)
{
    bool_stack_item  *temp = B->top;
    B->top = temp->next;
    free(temp);
}

void bool_Push(bool val, bool_stack *B)
{
    bool_stack_item  *temp = (bool_stack_item *)malloc(sizeof(bool_stack_item));
    if(temp == NULL)
        return;
    temp->value = val;
    temp->next = B->top;
    B->top = temp;
}

void bool_Top(bool *val, bool_stack *B)
{
    *val = B->top->value;
}

