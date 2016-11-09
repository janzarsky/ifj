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
    switch (top()) {
        case T_ID:
            pop();
            break;
        case NT_EXPR:
            pop();
            
            if (top() != T_PLUS && top() != T_MUL)
                return SYNTAX_ERROR;

            pop();

            if (top() != NT_EXPR)
                return SYNTAX_ERROR;
            
            pop();
            break;
        default:
            return SYNTAX_ERROR;
    }

    if (top() != TE_L)
        return SYNTAX_ERROR;
    
    pop();

    push(NT_EXPR);

    return SYNTAX_OK;
}

void print_stack() {
    stack_item_t *temp = stack.top;

    printf("top: ");

    while (temp != NULL) {
        switch (temp->symbol) {
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
                printf("%d ", temp->symbol);
                break;
        }
        temp = temp->next;
    }

    printf("bottom\n");
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
        printf("MATH_EXPR: top %d, b %d\n", top_term(), b);
        print_stack();

        switch (table[top_term()][b]) {
            case TE_E:
                printf("rule =\n");
                push(b);
                b = get_next_token();
                break;
            case TE_L:
                printf("rule <\n");
                insert_after_top_term(TE_L);
                push(b);
                b = get_next_token();
                break;
            case TE_R:
                printf("rule >\n");
                result = execute_rule();
                if (result == SYNTAX_ERROR) {
                    return SYNTAX_ERROR;
                }
                break;
            case TE_N:
            default:
                printf("rule none\n");
                return SYNTAX_ERROR;
                break;
        }

    } while (top_term() != T_END || b != T_END);

    print_stack();

    return SYNTAX_OK;
}
