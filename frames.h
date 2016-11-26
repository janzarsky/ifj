#ifndef _FRAMES_H
#define _FRAMES_H

#include "symtab.h"
#include "instrlist.h"
#include "interpret.h"

int call(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func);

int return_instr(symtab_t *symtab, tListOfInstr *instrlist);

st_value_t get_value(symtab_elem_t *var);
void set_value(symtab_elem_t *var, inter_value *value);

#endif
