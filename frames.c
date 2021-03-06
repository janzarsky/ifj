/**
 * Implementace interpretu imperativniho jazyka IFJ16
 * 
 * xzarsk03   Jan Zarsky
 * xvlcek23   David Vlcek
 * xpelan04   Pelantova Lucie
 * xmrlik00   Vit Mrlik
 * xpapla00   Andrei Paplauski
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instrlist.h"
#include "frames.h"
#include "interpret.h"
#include "string.h"
#include "ial.h"
#include "error_codes.h"

extern int ifj_errno;

frame_t *new_frame = NULL;
frame_t *active_frame = NULL;

int fr_add_item(frame_t *frame, symtab_elem_t *var) {
    frame_item_t *temp = malloc(sizeof(frame_item_t));

    if (temp == NULL)
        return ER_INTERN;

    temp->var = var;

    st_value_t null_val;
    null_val.ival = 0;
    temp->value = null_val;
    temp->initialized = false;

    temp->next = frame->first_item;
    frame->first_item = temp;

    return ER_OK;
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
        return ER_INTERN;

    frame_item_t *item = fr_get_item(frame, var);

    if (item == NULL) {
        int result = fr_add_item(frame, var);

        if (result != ER_OK)
            return result;

        item = fr_get_item(frame, var);

        if (item == NULL)
            return ER_INTERN;
    }

    item->value = val;
    item->initialized = true;

    return ER_OK;
}

int fr_get(frame_t *frame, symtab_elem_t *var, st_value_t *val) {
    if (frame == NULL)
        return ER_INTERN;

    frame_item_t *item = fr_get_item(frame, var);

    if (item == NULL) {
        int result = fr_add_item(frame, var);

        if (result != ER_OK)
            return result;

        item = fr_get_item(frame, var);

        if (item == NULL)
            return ER_RUN_OTHER;
    }

    if (!item->initialized)
        return ER_RUN_INIT;

    *val = item->value;

    return ER_OK;
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
        ifj_errno = ER_OK;

        int result = readInt();

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *)(unsigned long)result, stack);
    }
    else if (strcmp(func->id, "ifj16.readDouble") == 0) {
        double *result = malloc(sizeof(double));

        if (result == NULL)
            return ER_INTERN;

        ifj_errno = ER_OK;

        *result = readDouble();

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *) result, stack);
    }
    else if (strcmp(func->id, "ifj16.readString") == 0) {
        ifj_errno = ER_OK;

        char *result = readString();

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *) result, stack);
    }
    else if (strcmp(func->id, "ifj16.print") == 0) {
        inter_value param_value;
        stack_inter_Top(&param_value, stack);
        stack_inter_Pop(stack);

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

        ifj_errno = ER_OK;

        int result = length(param_value.union_value.strval);

        if (ifj_errno != ER_OK)
            return ifj_errno;

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

        ifj_errno = ER_OK;

        char * result = substr(param_s.union_value.strval,
            param_i.union_value.ival, param_n.union_value.ival);

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *) result, stack);
    }
    else if (strcmp(func->id, "ifj16.compare") == 0) {
        inter_value param_s2;
        stack_inter_Top(&param_s2, stack);
        stack_inter_Pop(stack);
        
        inter_value param_s1;
        stack_inter_Top(&param_s1, stack);
        stack_inter_Pop(stack);

        ifj_errno = ER_OK;

        int result = compare(param_s1.union_value.strval,
            param_s2.union_value.strval);

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *)(unsigned long) result, stack);
    }
    else if (strcmp(func->id, "ifj16.find") == 0) {
        inter_value param_search;
        stack_inter_Top(&param_search, stack);
        stack_inter_Pop(stack);
        
        inter_value param_s;
        stack_inter_Top(&param_s, stack);
        stack_inter_Pop(stack);

        ifj_errno = ER_OK;

        int result = find(param_s.union_value.strval,
            param_search.union_value.strval);

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *)(unsigned long) result, stack);
    }
    else if (strcmp(func->id, "ifj16.sort") == 0) {
        inter_value param_value;
        stack_inter_Top(&param_value, stack);
        stack_inter_Pop(stack);

        ifj_errno = ER_OK;

        char * result = sort(param_value.union_value.strval);

        if (ifj_errno != ER_OK)
            return ifj_errno;

        push_val((void *) result, stack);
    }

    return ER_OK;
}

int call_instr(tListOfInstr *instrlist, inter_stack *stack, symtab_elem_t *func) {
    if (func->elem_type == ST_ELEMTYPE_FUN) {
        new_frame = malloc(sizeof(frame_t));

        if (new_frame == NULL)
            return ER_INTERN;

        new_frame->first_item = NULL;
        
        symtab_elem_t *param = func->last_param;

        while (param != NULL) {
            fr_add_item(new_frame, param);

            inter_value param_value;
            stack_inter_Top(&param_value, stack);
            stack_inter_Pop(stack);
            fr_set(new_frame, param, param_value.union_value);

            param = param->prev_param;
        }

        new_frame->next = active_frame;
        active_frame = new_frame;
        new_frame = NULL;

        if (strcmp(func->id, "run") == 0) {
            active_frame->next_instr = NULL;
        }
        else {
            active_frame->next_instr = instrlist->active->nextItem;
        }

        listGoto(instrlist, func->first_instr);
    }
    else if (func->elem_type == ST_ELEMTYPE_BUILTIN) {
        int result = call_builtin_function(stack, func);
        
        if (result != ER_OK)
            return result;

        listNext(instrlist);
    }

    return ER_OK;
}

int return_instr(tListOfInstr *instrlist) {
    if (active_frame == NULL)
        return ER_INTERN;

    listGoto(instrlist, active_frame->next_instr);

    frame_t *temp = active_frame;
    active_frame = active_frame->next;

    fr_free(&temp);

    if (active_frame == NULL)
        return FR_NO_FRAMES;

    return ER_OK;
}

st_value_t get_value(symtab_elem_t *var) {
    if (var->is_global) {
        if (!var->initialized) {
            ifj_errno = ER_RUN_INIT;
            st_value_t temp;
            return temp;
        }

        return var->value;
    }

    st_value_t val;
    
    int result = fr_get(active_frame, var, &val);

    if (result != ER_OK)
        ifj_errno = result;

    return val;
}

void set_value(symtab_elem_t *var, inter_value *value) {
    if (var->is_global) {
        var->value = value->union_value;
        var->initialized = true;
    }
    else {
        fr_set(active_frame, var, value->union_value);
    }
}
