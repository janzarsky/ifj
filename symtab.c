#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser_test.h"
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

    if (tabulka->elements[klic] != NULL) {
        synon->nextElem = tabulka->elements[klic]->nextElem;
        tabulka->elements[klic]->nextElem = synon;
    }
    else {
        synon->nextElem = NULL;
        tabulka->elements[klic] = synon;
    }

    synon->is_global = false;

    return synon;
}

symtab_elem_t *st_find (symtab_t *tabulka, char *token){
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
    for (int i = 0; i <TABLE_SIZE; i++)
    {
        if (table->elements[i] != NULL) {
            while (table->elements[i]->nextElem != NULL) {
                ptr = table->elements[i]->nextElem;
                table->elements[i]->nextElem = table->elements[i]->nextElem->nextElem;
                free(ptr);
            }

            free(table->elements[i]);
        }
    }
    free(table);
}

#ifdef DEBUG
void st_print_elem(symtab_elem_t *elem, char *prefix) {
    printf("%saddr: %p, id: %s, elem_type: ", prefix, (void *)elem, elem->id);

    switch (elem->elem_type) {
        case ST_ELEMTYPE_VAR:
            printf("var"); break;
        case ST_ELEMTYPE_FUN:
            printf("fun"); break;
        case ST_ELEMTYPE_BUILTIN:
            printf("builtin"); break;
        case ST_ELEMTYPE_PARAM:
            printf("param"); break;
        case ST_ELEMTYPE_CLASS:
            printf("class"); break;
        default:
            printf("other"); break;
    }

    printf(", data_type: ");

    switch (elem->data_type) {
        case ST_DATATYPE_ERROR:
            printf("error"); break;
        case ST_DATATYPE_VOID:
            printf("void"); break;
        case ST_DATATYPE_INT:
            printf("int, value: %d", elem->value.ival); break;
        case ST_DATATYPE_DOUBLE:
            if (elem->value.dval != NULL)
                printf("double, value: %g", *(elem->value.dval));
            else
                printf("double, value: NULL");
            break;
        case ST_DATATYPE_STRING:
            printf("string, value: %s", elem->value.strval); break;
        case ST_DATATYPE_BOOL:
            printf("bool"); break;
        default:
            printf("other"); break;
    }

    printf(", local_table: %p, first_instr: %p\n", (void *)elem->local_table, (void *)elem->first_instr);
}

void st_print(symtab_t *table) {
    symtab_elem_t *ptr;
    symtab_elem_t *ptr2;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table->elements[i] != NULL) {
            ptr = table->elements[i];

            while (ptr != NULL) {
                st_print_elem(ptr, "");

                if (ptr->local_table != NULL) {
                    for (int j = 0; j < TABLE_SIZE; j++) {
                        if (ptr->local_table->elements[j] != NULL) {
                            ptr2 = ptr->local_table->elements[j]; 

                            while (ptr2 != NULL) {
                                st_print_elem(ptr2, "-> ");
                                ptr2 = ptr2->nextElem;
                            }
                        }
                    }
                }

                ptr = ptr->nextElem;
            }
        }
    }
}
#endif

symtab_elem_t *st_add_builtin(symtab_t *table, char* name, int type) {
    char *name_dup = strdup(name);

    symtab_elem_t *temp = st_add(table, name_dup);
    temp->elem_type = ST_ELEMTYPE_BUILTIN;
    temp->data_type = type;
    temp->first_param = NULL;

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

    temp->first_param = st_add(temp->local_table, "s");
    temp->first_param->elem_type = ST_ELEMTYPE_PARAM;
    temp->first_param->data_type = ST_DATATYPE_STRING;
    temp->first_param->next_param = NULL;
}
