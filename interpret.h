#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include "symtab.h"

typedef union {
    st_value_t union_value;
    void *vval;
} inter_value;

typedef struct inter_stack_item {
    inter_value value;
    struct inter_stack_item *next;
}inter_stack_item;

typedef struct inter_stack{
    inter_stack_item *top;
   } inter_stack;

typedef struct bool_stack_item {
    bool value;
    struct bool_stack_item *next;
}bool_stack_item;

typedef struct bool_stack{
    bool_stack_item *top;
   } bool_stack;

int interpret(symtab_t *T, tListOfInstr *L);

void stack_inter_Pop(inter_stack *S);
void push_tab(st_value_t val, inter_stack *S);
void push_val(void *val, inter_stack *S);
void stack_inter_Top(inter_value *val, inter_stack *S);
void bool_Pop(bool_stack *B);
void bool_Push(bool val, bool_stack *B);
void bool_Top(bool *val, bool_stack *B);

#endif // INTERPRET_H_INCLUDED
