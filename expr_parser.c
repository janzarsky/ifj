#include <stdio.h>
#include <stdlib.h>
#include "parser_test.h"
#include "instrlist.h"

int token;

enum token_t { T_PLUS, T_MUL, T_LEFT_BRACKET, T_RIGHT_BRACKET, T_ID, T_END, T_MAX };
enum nonterm_t { NT_EXPR = T_MAX, NT_MAX };
enum table_entry_t { TE_N = NT_MAX, TE_L, TE_E, TE_R, TE_MAX }; // none, <, =, >

const char table[T_MAX][T_MAX] = {
    { TE_R, TE_L, TE_L, TE_R, TE_L, TE_R },
    { TE_R, TE_R, TE_L, TE_R, TE_L, TE_R },
    { TE_L, TE_L, TE_L, TE_E, TE_L, TE_N },
    { TE_R, TE_R, TE_N, TE_R, TE_N, TE_R },
    { TE_R, TE_R, TE_N, TE_R, TE_N, TE_R },
    { TE_L, TE_L, TE_L, TE_N, TE_L, TE_N }
};

typedef struct stack_item {
    int symbol;
    struct stack_item *next;
} stack_item_t;

typedef struct stack{
    stack_item_t *top;
} stack_t;

stack_t stack = { NULL };

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
        return T_END;
    }
}

int is_term(int symbol) {
    return (symbol < T_MAX);
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

    return T_END;
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

int execute_rule() {
    // rule E -> E+E
    if (stack.top->symbol == NT_EXPR
        && stack.top->next->symbol == T_PLUS
        && stack.top->next->next->symbol == NT_EXPR) {
        printf("rule: E -> E+E, ");
        pop_n_times(4);
        push(NT_EXPR);
    }
    // rule E -> E*E
    else if (stack.top->symbol == NT_EXPR
        && stack.top->next->symbol == T_MUL
        && stack.top->next->next->symbol == NT_EXPR) {
        printf("rule: E -> E*E, ");
        pop_n_times(4);
        push(NT_EXPR);
    }
    // rule E -> (E)
    else if (stack.top->symbol == T_LEFT_BRACKET
        && stack.top->next->symbol == NT_EXPR
        && stack.top->next->next->symbol == T_RIGHT_BRACKET) {
        printf("rule: E -> (E), ");
        pop_n_times(4);
        push(NT_EXPR);
    }
    // rule E -> ID
    else if (stack.top->symbol == T_ID) {
        printf("rule: E -> ID, ");
        pop_n_times(2);
        push(NT_EXPR);
    }
    else {
        return SYNTAX_ERROR;
    }

    return SYNTAX_OK;
}

void print_symbol(int symbol) {
    switch (symbol) {
        case T_PLUS:
            printf("+ ");
            break;
        case T_MUL:
            printf("* ");
            break;
        case T_LEFT_BRACKET:
            printf("( ");
            break;
        case T_RIGHT_BRACKET:
            printf(") ");
            break;
        case T_ID:
            printf("i ");
            break;
        case T_END:
            printf("$ ");
            break;
        case TE_L:
            printf("< ");
            break;
        case TE_R:
            printf("> ");
            break;
        case NT_EXPR:
            printf("E ");
            break;
        default:
            printf("%d ", symbol);
            break;
    }
}

void print_stack_item(stack_item_t *item) {
    if (item->next != NULL) {
        print_stack_item(item->next);
    }

    print_symbol(item->symbol);
}

void print_stack() {
    print_stack_item(stack.top);
}

int bool_expr() {
    return SYNTAX_OK;
}

extern int get_next_token();

int math_expr() {
    int b, result;

    push(T_END);

    b = get_next_token();
    
    do {
        printf("stack: ");
        print_stack();
        printf(" \t\tinput: ");
        print_symbol(b);
        printf(", ");

        switch (table[top_term()][b]) {
            case TE_E:
                printf("op: =, ");
                push(b);
                b = get_next_token();
                break;
            case TE_L:
                printf("op: <, ");
                insert_after_top_term(TE_L);
                push(b);
                b = get_next_token();
                break;
            case TE_R:
                printf("op: >, ");
                result = execute_rule();
                if (result == SYNTAX_ERROR) {
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

    } while (top_term() != T_END || b != T_END);

    printf("stack: ");
    print_stack();
    printf(" \t\tinput: ");
    print_symbol(b);
    printf("\n");

    return SYNTAX_OK;
}
