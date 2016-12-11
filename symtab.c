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
#include <stdbool.h>

#include "parser.h"
#include "symtab.h"

unsigned int hash_function(const char *str, unsigned htab_size) {
    unsigned int h = 0;
    const unsigned char *p;
    for(p = (const unsigned char *)str; *p != '\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
}

int st_init(symtab_t **table){
    *table = (symtab_t *) malloc(sizeof(symtab_elem_t)*TABLE_SIZE);

    if (*table == NULL)
        return - 1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        (*table)->elements[i] = NULL;
    }

    return 0;
}

symtab_elem_t *st_add(symtab_t *tabulka, char *token) {
    unsigned int klic = hash_function(token, TABLE_SIZE);

    symtab_elem_t *synon = (symtab_elem_t *) malloc(sizeof(symtab_elem_t));

    if (synon == NULL)
        return NULL;

    synon->id = strdup(token);

    synon->nextElem = tabulka->elements[klic];
    tabulka->elements[klic] = synon;
    
    synon->is_global = false;
    synon->first_param = NULL;
    synon->last_param = NULL;
    synon->next_param = NULL;
    synon->prev_param = NULL;
    synon->local_table = NULL;

    return synon;
}

symtab_elem_t *st_find (symtab_t *tabulka, char *token){
    if (tabulka == NULL)
        return NULL;

    unsigned int klic = hash_function(token, TABLE_SIZE);

    symtab_elem_t *ptr = tabulka->elements[klic];

    while (ptr != NULL) {
        if (strcmp(ptr->id, token) == 0)
            return ptr;

        ptr = ptr->nextElem;
    }

    return NULL;
}

symtab_elem_t *st_find_global (symtab_t *tabulka, char *token, char *class){
    symtab_elem_t *ptr = st_find(tabulka, token);

    if (ptr != NULL)
        return ptr;
    
    char * str = str_conc(class, token);

    ptr = st_find(tabulka, str);

    return ptr;
}

void st_free(symtab_t *table) {
    symtab_elem_t *ptr;

    for (int i = 0; i < TABLE_SIZE; i++) {
        while (table->elements[i] != NULL) {
            ptr = table->elements[i];
            table->elements[i] = ptr->nextElem;

            if (ptr->local_table != NULL) {
                st_free(ptr->local_table);
            }

            free(ptr->id);
            free(ptr);
        }
    }

    free(table);
} 
void st_add_builtin_param(symtab_elem_t *elem, char *name, st_datatype_t data_type) {
    symtab_elem_t *temp = elem->first_param;

    elem->first_param = st_add(elem->local_table, name);
    elem->first_param->elem_type = ST_ELEMTYPE_PARAM;
    elem->first_param->data_type = data_type;
    elem->first_param->next_param = temp;
    elem->first_param->prev_param = NULL;

    if (temp != NULL)
        temp->prev_param = elem->first_param;
    else
        elem->last_param = elem->first_param;
}

symtab_elem_t *st_add_builtin(symtab_t *table, char* name, int type) {
    symtab_elem_t *temp = st_add(table, name);
    temp->elem_type = ST_ELEMTYPE_BUILTIN;
    temp->data_type = type;
    temp->is_global = true;
    temp->first_param = NULL;
    temp->last_param = NULL;

    return temp;
}

void st_add_builtin_functions(symtab_t *table) {
    symtab_elem_t *temp;

    temp = st_add(table, "ifj16");
    temp->elem_type = ST_ELEMTYPE_CLASS;

    st_add_builtin(table, "ifj16.readInt", ST_DATATYPE_INT);
    st_add_builtin(table, "ifj16.readDouble", ST_DATATYPE_DOUBLE);
    st_add_builtin(table, "ifj16.readString", ST_DATATYPE_STRING);

    temp = st_add_builtin(table, "ifj16.print", ST_DATATYPE_VOID);
    st_init(&(temp->local_table));
    st_add_builtin_param(temp, "s", ST_DATATYPE_STRING);

    temp = st_add_builtin(table, "ifj16.length", ST_DATATYPE_INT);
    st_init(&(temp->local_table));
    st_add_builtin_param(temp, "s", ST_DATATYPE_STRING);

    temp = st_add_builtin(table, "ifj16.substr", ST_DATATYPE_STRING);
    st_init(&(temp->local_table));
    // reverse order needed
    st_add_builtin_param(temp, "n", ST_DATATYPE_INT);
    st_add_builtin_param(temp, "i", ST_DATATYPE_INT);
    st_add_builtin_param(temp, "s", ST_DATATYPE_STRING);

    temp = st_add_builtin(table, "ifj16.compare", ST_DATATYPE_INT);
    st_init(&(temp->local_table));
    st_add_builtin_param(temp, "s2", ST_DATATYPE_STRING);
    st_add_builtin_param(temp, "s1", ST_DATATYPE_STRING);

    temp = st_add_builtin(table, "ifj16.find", ST_DATATYPE_INT);
    st_init(&(temp->local_table));
    st_add_builtin_param(temp, "search", ST_DATATYPE_STRING);
    st_add_builtin_param(temp, "s", ST_DATATYPE_STRING);

    temp = st_add_builtin(table, "ifj16.sort", ST_DATATYPE_STRING);
    st_init(&(temp->local_table));
    st_add_builtin_param(temp, "s", ST_DATATYPE_STRING);
}
