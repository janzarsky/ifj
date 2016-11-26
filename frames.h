#ifndef _FRAMES_H
#define _FRAMES_H

#include "symtab.h"
#include "instrlist.h"
#include "interpret.h"

int call(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func);

#endif
