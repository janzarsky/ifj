#ifndef _FRAMES_H
#define _FRAMES_H

#include <stdbool.h>
#include "symtab.h"
#include "instrlist.h"
#include "interpret.h"

#define FR_NO_FRAMES (ER_INTERN + 1)

typedef struct frame_item_t {
    symtab_elem_t *var;
    st_value_t value;
    bool initialized;
    struct frame_item_t *next;
} frame_item_t;

typedef struct frame_t {
    frame_item_t *first_item;
    tListItem *next_instr;
    struct frame_t *next;
} frame_t;

int call_instr(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func);
int return_instr(tListOfInstr *instrlist);

st_value_t get_value(symtab_elem_t *var);
void set_value(symtab_elem_t *var, inter_value *value);

#endif
