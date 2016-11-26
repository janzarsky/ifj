#include <stdio.h>

#include "instrlist.h"
#include "frames.h"
#include "interpret.h"
#include "parser_test.h"

int call_instr(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func) {
    printf("inside call instruction, params: %p %p %p\n", (void *) instrlist, (void *) stack, (void *) func);
    return SYNTAX_OK;
}

int return_instr(symtab_t *symtab, tListOfInstr *instrlist) {
    printf("inside return instruction, params: %p %p\n", (void *) symtab, (void *) instrlist);
    return SYNTAX_OK;
}

st_value_t get_value(symtab_elem_t *var) {
    return var->value;
}

void set_value(symtab_elem_t *var, inter_value *value) {
    var->value = value->union_value;
}
