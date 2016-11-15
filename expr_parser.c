#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "parser_test.h"
#include "instrlist.h"

int token;

#define DEBUG_PRINT_STACK_WIDTH 30
int debug_print_cnt = 0;

enum nonterm_t { NT_EXPR = TOKEN_MAX, NT_MAX };
enum table_entry_t { TE_N = NT_MAX, TE_L, TE_E, TE_R, TE_MAX }; // none, <, =, >

// FIXME use constants instead of literal
const char table[8][8] = {
//           +     -     *     /     (     )     ID    $
/* +   */ { TE_R, TE_R, TE_L, TE_L, TE_L, TE_R, TE_L, TE_R },
/* -   */ { TE_R, TE_R, TE_L, TE_L, TE_L, TE_R, TE_L, TE_R },
/* *   */ { TE_R, TE_R, TE_R, TE_R, TE_L, TE_R, TE_L, TE_R },
/* /   */ { TE_R, TE_R, TE_R, TE_R, TE_L, TE_R, TE_L, TE_R },
/* (   */ { TE_L, TE_L, TE_L, TE_L, TE_L, TE_E, TE_L, TE_N },
/* )   */ { TE_R, TE_R, TE_R, TE_R, TE_N, TE_R, TE_N, TE_R },
/* ID  */ { TE_R, TE_R, TE_R, TE_R, TE_N, TE_R, TE_N, TE_R },
/* $   */ { TE_L, TE_L, TE_L, TE_L, TE_L, TE_N, TE_L, TE_N }
};

// template
///* x   */ { TE_ , TE_ , TE_ , TE_ , TE_ , TE_ , TE_  },

typedef struct stack_item {
    int symbol;
    struct stack_item *next;
} stack_item_t;

typedef struct stack{
    stack_item_t *top;
} stack_t;

stack_t stack = { NULL };

tListOfInstr instr_list;

int map_token(int token) {
    switch (token) {
        case PLUS:
            return 0; break;
        case MINUS:
            return 1; break;
        case MUL:
            return 2; break;
        case DIV:
            return 3; break;
        case LEFT_BRACKET:
            return 4; break;
        case RIGHT_BRACKET:
            return 5; break;
        case ID:
            return 6; break;
        case END_OF_FILE:
            return 7; break;
        default:
            return -1;
    }
}

void push(int symbol) {
    stack_item_t *temp = malloc(sizeof(stack_item_t));

    if (temp == NULL)
        return;

    temp->next = stack.top;
    temp->symbol = symbol;

    stack.top = temp;
}

void pop() {
    if (stack.top != NULL) {
        stack_item_t *temp = stack.top;
        stack.top = temp->next;
        free(temp);
    }
}

void pop_n_times(int n) {
    while (n > 0) {
        pop();
        n--;
    }
}

int top() {
    if (stack.top != NULL) {
        return stack.top->symbol;
    }
    else {
        return END_OF_FILE;
    }
}

int is_term(int symbol) {
    return (symbol < TOKEN_MAX);
}

int top_term() {
    stack_item_t *temp = stack.top;

    while (temp != NULL) {
        if (is_term(temp->symbol)) {
            return temp->symbol;
        }
        else {
            temp = temp->next;
        }
    }

    return END_OF_FILE;
}

void insert_after_top_term(int symbol) {
    stack_item_t *temp = stack.top;
    stack_item_t *prev = NULL;

    while (temp != NULL) {
        if (is_term(temp->symbol)) {
            stack_item_t *new_item = malloc(sizeof(stack_item_t));

            if (new_item == NULL)
                return;

            new_item->symbol = symbol;
            new_item->next = temp;
            
            if (prev == NULL) {
                stack.top = new_item;
            }
            else {
                prev->next = new_item;
            }

            return;
        }
        else {
            prev = temp;
            temp = temp->next;
        }
    }
}

void print_instr(tInstr *instr) {
    switch (instr->instType) {
        case IN_ADD:
            printf("ADD"); break;
        case IN_SUB:
            printf("SUB"); break;
        case IN_MUL:
            printf("MUL"); break;
        case IN_DIV:
            printf("DIV"); break;
        case IN_PUSH:
            printf("PUSH"); break;
        default:
            printf("%d", instr->instType);
    }

    printf(" %p %p %p", instr->addr1, instr->addr2, instr->addr3);
}

void add_instr(int type, void * ptr1, void * ptr2, void * ptr3) {
    tInstr instr = { type, ptr1, ptr2, ptr3 };

    listInsertLast(&instr_list, instr);

    printf("instr: ");

    print_instr(&instr);
}

//         E     -> E        +     E
// rule(4, NT_EXPR, NT_EXPR, PLUS, NT_EXPR);
bool rule(int num, ...) {
    va_list valist;
    va_start(valist, num);

    // left part of a rule
    int left_symbol = va_arg(valist, int);

    // copy arguments (we need them in reverse order)
    int *symbols = malloc((num-1)*sizeof(int));

    if (symbols == NULL) {
        va_end(valist);
        return false;
    }

    for (int i = 0; i < num - 1; i++) {
        symbols[num - 2 - i] = va_arg(valist, int);
    }
    
    // compare stack with arguments
    stack_item_t *temp = stack.top;

    for (int i = 0; i < num - 1; i++) {
        if (temp->symbol != symbols[i]) {
            va_end(valist);
            return false;
        }

        if (temp->next == NULL) {
            va_end(valist);
            return false;
        }

        temp = temp->next;
    }

    if (temp->symbol != TE_L) {
        va_end(valist);
        return false;
    }

    pop_n_times(num);

    push(left_symbol);

    va_end(valist);
    return true;
}

int rules() {
    if (rule(4, NT_EXPR, NT_EXPR, PLUS, NT_EXPR)) {
        printf("rule: E -> E+E    ");
        add_instr(IN_ADD, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, MINUS, NT_EXPR)) {
        printf("rule: E -> E-E    ");
        add_instr(IN_SUB, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, MUL, NT_EXPR)) {
        printf("rule: E -> E*E    ");
        add_instr(IN_MUL, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, DIV, NT_EXPR)) {
        printf("rule: E -> E/E    ");
        add_instr(IN_DIV, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, LEFT_BRACKET, NT_EXPR, RIGHT_BRACKET)) {
        printf("rule: E -> (E)    ");
    }
    else if (rule(2, NT_EXPR, ID)) {
        printf("rule: E -> ID     ");
        add_instr(IN_PUSH, (void *) 0x42, NULL, NULL);
    }
    else {
        printf("rule: no matching rule");
        return SYNTAX_ERROR;
    }

    return SYNTAX_OK;
}

void print_symbol(int symbol) {
    switch (symbol) {
        case PLUS:
            printf("+");
            break;
        case MINUS:
            printf("-");
            break;
        case MUL:
            printf("*");
            break;
        case DIV:
            printf("/");
            break;
        case LEFT_BRACKET:
            printf("(");
            break;
        case RIGHT_BRACKET:
            printf(")");
            break;
        case ID:
            printf("i");
            break;
        case END_OF_FILE:
            printf("$");
            break;
        case TE_L:
            printf("<");
            break;
        case TE_R:
            printf(">");
            break;
        case NT_EXPR:
            printf("E");
            break;
        default:
            printf("%d", symbol);
            break;
    }
}

void print_stack_item(stack_item_t *item) {
    if (item->next != NULL) {
        print_stack_item(item->next);
    }

    print_symbol(item->symbol);
    debug_print_cnt++;
}

void print_stack() {
    debug_print_cnt = 0;

    print_stack_item(stack.top);

    for (int i = debug_print_cnt; i < DEBUG_PRINT_STACK_WIDTH; i++)
        printf(" ");
}

void print_instr_list() {
    tInstr *instr;

    listFirst(&instr_list);

    while (instr_list.active != NULL) {
        instr = listGetData(&instr_list);

        print_instr(instr);
        printf("\n");

        listNext(&instr_list);
    }
}

extern int get_next_token();

int math_expr() {
    int b, result;

    listInit(&instr_list);

    push(END_OF_FILE);

    b = get_next_token();
    
    do {
        printf("stack: ");
        print_stack();
        printf("    input: ");
        print_symbol(b);
        printf("    ");

        switch (table[map_token(top_term())][map_token(b)]) {
            case TE_E:
                printf("op: =    ");
                push(b);
                b = get_next_token();
                break;
            case TE_L:
                printf("op: <    ");
                insert_after_top_term(TE_L);
                push(b);
                b = get_next_token();
                break;
            case TE_R:
                printf("op: >    ");
                result = rules();
                if (result == SYNTAX_ERROR) {
                    printf("\n");
                    return SYNTAX_ERROR;
                }
                break;
            case TE_N:
            default:
                printf("op: none, ");
                return SYNTAX_ERROR;
                break;
        }

        printf("\n");

    } while (top_term() != END_OF_FILE || b != END_OF_FILE);

    printf("stack: ");
    print_stack();
    printf("    input: ");
    print_symbol(b);
    printf("\n");

    printf("\n");
    printf("Generated instructions:\n");
    print_instr_list();

    return SYNTAX_OK;
}

int bool_expr() {
    return math_expr();
}

