#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instrlist.h"
#include "frames.h"
#include "interpret.h"
#include "string.h"
#include "ial.h"
#include "error_codes.h"
#include "debug.h"

frame_t *new_frame = NULL;
frame_t *active_frame = NULL;

#ifdef DEBUG
void fr_print(frame_t *frame) {
    printf("frame: next_instr: %p ( ", (void *) frame->next_instr);
    
    if (frame->next_instr != NULL)
        print_instr(&(frame->next_instr->instruction));

    printf(" )\n");

    frame_item_t *temp = frame->first_item;

    while (temp != NULL) {
        if (temp->var != NULL)
            printf("frame_item: var: %p, id: %s, value: %i\n",
                (void *)temp->var, temp->var->id, temp->value.ival);
        else
            printf("some error/n");
        temp = temp->next;
    }
}

void fr_print_frames() {
    if (new_frame != NULL) {
        printf("INTERPRET: new_frame: \n");
        fr_print(new_frame);
    }

    frame_t *temp = active_frame;

    if (temp == NULL)
        printf("INTERPRET: no active_frame\n");

    while (temp != NULL) {
        printf("INTERPRET: active_frame: \n");
        fr_print(temp);
        temp = temp->next;
    }
}
#endif

int fr_add_item(frame_t *frame, symtab_elem_t *var) {
    frame_item_t *temp = malloc(sizeof(frame_item_t));

    if (temp == NULL)
        return INTERNAL_ERROR;

    temp->var = var;

    st_value_t null_val;
    null_val.ival = 0;
    temp->value = null_val;

    temp->next = frame->first_item;
    frame->first_item = temp;

    return 0;
}

frame_item_t *fr_get_item(frame_t *frame, symtab_elem_t *var) {
    frame_item_t *temp = frame->first_item;

    while (temp != NULL) {
        if (temp->var == var)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

int fr_set(frame_t *frame, symtab_elem_t *var, st_value_t val) {
    if (frame == NULL)
        return INTERNAL_ERROR;

    frame_item_t *item = fr_get_item(frame, var);

    if (item == NULL) {
        if (fr_add_item(frame, var) == INTERNAL_ERROR)
            return INTERNAL_ERROR;

        item = fr_get_item(frame, var);

        if (item == NULL)
            return INTERNAL_ERROR;
    }

    item->value = val;

    return 0;
}

int fr_get(frame_t *frame, symtab_elem_t *var, st_value_t *val) {
    if (frame == NULL)
        return INTERNAL_ERROR;

    frame_item_t *item = fr_get_item(frame, var);

    if (item == NULL) {
        if (fr_add_item(frame, var) == INTERNAL_ERROR)
            return INTERNAL_ERROR;

        item = fr_get_item(frame, var);

        if (item == NULL)
            return INTERNAL_ERROR;
    }

    *val = item->value;

    return 0;
}

void fr_free(frame_t **frame) {
    frame_item_t *item = (*frame)->first_item;

    while (item != NULL) {
        frame_item_t *temp = item;
        item = item->next;
        free(temp);
    }

    free(*frame);
    *frame = NULL;
}

int call_builtin_function(inter_stack *stack, symtab_elem_t *func) {
    if (strcmp(func->id, "ifj16.readInt") == 0) {
        int result = readInt();
        push_val((void *)(unsigned long)result, stack);
    }
    else if (strcmp(func->id, "ifj16.readDouble") == 0) {
        double *result = malloc(sizeof(double));

        if (result == NULL)
            return ER_INTERN;

        *result = readDouble();

        push_val((void *) result, stack);
    }
    else if (strcmp(func->id, "ifj16.readString") == 0) {
        char *result = readString();
        push_val((void *) result, stack);
    }
    else if (strcmp(func->id, "ifj16.print") == 0) {
        inter_value param_value;
        stack_inter_Top(&param_value, stack);
        stack_inter_Pop(stack);
        debug_printf("##### builtin function ifj16.print(): ");

        bool backslash = false;

        for (int i = 0; param_value.union_value.strval[i] != 0; i++) {
            if (backslash) {
                backslash = false;
                
                if (param_value.union_value.strval[i] == 'n')
                    putchar('\n');
            }
            else if (param_value.union_value.strval[i] == '\\') {
                backslash = true;
            }
            else {
                putchar(param_value.union_value.strval[i]);
            }
        }
    }
    else if (strcmp(func->id, "ifj16.length") == 0) {
        inter_value param_value;
        stack_inter_Top(&param_value, stack);
        stack_inter_Pop(stack);

        int result = length(param_value.union_value.strval);
        push_val((void *)(unsigned long) result, stack);
    }
    else if (strcmp(func->id, "ifj16.substr") == 0) {
        inter_value param_n;
        stack_inter_Top(&param_n, stack);
        stack_inter_Pop(stack);

        inter_value param_i;
        stack_inter_Top(&param_i, stack);
        stack_inter_Pop(stack);

        inter_value param_s;
        stack_inter_Top(&param_s, stack);
        stack_inter_Pop(stack);

        char * result = substr(param_s.union_value.strval,
            param_i.union_value.ival, param_n.union_value.ival);
        push_val((void *) result, stack);
    }
    else if (strcmp(func->id, "ifj16.compare") == 0) {
        inter_value param_s2;
        stack_inter_Top(&param_s2, stack);
        stack_inter_Pop(stack);
        
        inter_value param_s1;
        stack_inter_Top(&param_s1, stack);
        stack_inter_Pop(stack);

        int result = compare(param_s1.union_value.strval,
            param_s2.union_value.strval);
        push_val((void *)(unsigned long) result, stack);
    }
    else if (strcmp(func->id, "ifj16.find") == 0) {
        inter_value param_search;
        stack_inter_Top(&param_search, stack);
        stack_inter_Pop(stack);
        
        inter_value param_s;
        stack_inter_Top(&param_s, stack);
        stack_inter_Pop(stack);

        int result = find(param_s.union_value.strval,
            param_search.union_value.strval);
        push_val((void *)(unsigned long) result, stack);
    }
    else if (strcmp(func->id, "ifj16.sort") == 0) {
        inter_value param_value;
        stack_inter_Top(&param_value, stack);
        stack_inter_Pop(stack);

        char * result = sort(param_value.union_value.strval);
        push_val((void *) result, stack);
    }

#ifdef DEBUG
    stack_inter_print(stack);
#endif

    return ER_OK;
}

int call_instr(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func) {
    debug_printf("INTERPRET: inside call instruction, params: %p %p %p\n", (void *) instrlist, (void *) stack, (void *) func);

    if (func->elem_type == ST_ELEMTYPE_FUN) {
        new_frame = malloc(sizeof(frame_t));

        if (new_frame == NULL)
            return INTERNAL_ERROR;

        symtab_elem_t *param = func->first_param;

        while (param != NULL) {
            fr_add_item(new_frame, param);

            inter_value param_value;
            stack_inter_Top(&param_value, stack);
            stack_inter_Pop(stack);
            fr_set(new_frame, param, param_value.union_value);

            debug_printf("INTERPRET: adding param to frame, id: %s\n", param->id);
            param = param->next_param;
        }

        new_frame->next = active_frame;
        active_frame = new_frame;
        new_frame = NULL;

        if (strcmp(func->id, "run") == 0) {
            debug_printf("!!!! setting next_instr to %p\n", (void *) NULL);
            active_frame->next_instr = NULL;
        }
        else {
            debug_printf("!!!! setting next_instr to %p\n", (void *)instrlist->active->nextItem);
            active_frame->next_instr = instrlist->active->nextItem;
        }
        debug_printf("!!!! going to %p\n", (void *)func->first_instr);
        listGoto(instrlist, func->first_instr);
    }
    else if (func->elem_type == ST_ELEMTYPE_BUILTIN) {
        debug_printf("***** calling builtin function %s\n", func->id);

        call_builtin_function(stack, func);

        listNext(instrlist);
    }

#ifdef DEBUG
    fr_print_frames();
#endif

    return ER_OK;
}

int return_instr(tListOfInstr *instrlist) {
    debug_printf("inside return instruction, %p\n", (void *) instrlist);

#ifdef DEBUG
    fr_print_frames();
#endif

    if (active_frame == NULL)
        return ER_INTERN;

    debug_printf("!!!! going to %p\n", (void *)active_frame->next_instr);
    listGoto(instrlist, active_frame->next_instr);

    frame_t *temp = active_frame;
    active_frame = active_frame->next;

    fr_free(&temp);

#ifdef DEBUG
    fr_print_frames();
#endif

    if (active_frame == NULL) {
        debug_printf("!!!! no frames\n");
        return FR_NO_FRAMES;
    }

    return ER_OK;
}

st_value_t get_value(symtab_elem_t *var) {
    if (var->is_global)
        return var->value;

    st_value_t val;
    fr_get(active_frame, var, &val);
    return val;
}

void set_value(symtab_elem_t *var, inter_value *value) {
    if (var->is_global)
        var->value = value->union_value;
    else
        fr_set(active_frame, var, value->union_value);

#ifdef DEBUG
    fr_print_frames();
#endif
}
