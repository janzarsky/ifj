#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

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

void stack_inter_Pop(inter_stack *S);

void push_tab(st_value_t val, inter_stack *S);

void push_val(void *val, inter_stack *S);

void stack_inter_Top(inter_value *val, inter_stack *S);

#endif // INTERPRET_H_INCLUDED
