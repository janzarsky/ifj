#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "parser_test.h"
#include "instrlist.h"

int token;

#ifdef DEBUG

#define DEBUG_PRINT_STACK_WIDTH 40
int debug_print_cnt = 0;

#define debug_printf(...) printf(__VA_ARGS__)

#else

#define debug_printf(...) ;

#endif

enum nonterm_t { NT_EXPR = TOKEN_MAX, NT_MAX };
enum table_entry_t { T_N = NT_MAX, T_L, T_E, T_R, T_MAX }; // none, <, =, >

/* Priority:
 * 
 * 3: * /
 * 4: + -
 * 6: < > <= >=
 * 7: == != 
 */

#define TABLE_SIZE 7

const char table[TABLE_SIZE][TABLE_SIZE] = {
//             0    1    2    3    4    5    6
//             + -  * /  rel  (    )    ID   $
/*  0 + - */ { T_R, T_L, T_R, T_L, T_R, T_L, T_R },
/*  1 * / */ { T_R, T_R, T_R, T_L, T_R, T_L, T_R },
/*  2 rel */ { T_L, T_L, T_N, T_L, T_R, T_L, T_R },
/*  3 (   */ { T_L, T_L, T_L, T_L, T_E, T_L, T_N },
/*  4 )   */ { T_R, T_R, T_R, T_N, T_R, T_N, T_R },
/*  5 id  */ { T_R, T_R, T_R, T_N, T_R, T_N, T_R },
/*  6 $   */ { T_L, T_L, T_L, T_L, T_N, T_L, T_N }
};

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
        case MINUS:
            return 0; break;
        case MUL:
        case DIV:
            return 1; break;
        case LESS:
        case GREAT:
        case LESS_EQ:
        case GREAT_EQ:
        case EQUAL:
        case N_EQUAL:
            return 2; break;
        case LEFT_BRACKET:
            return 3; break;
        case RIGHT_BRACKET:
            return 4; break;
        case INT_LITERAL:
        case DOUBLE_LITERAL:
        case STRING_LITERAL:
        case ID:
            return 5; break;
        case END_OF_FILE:
            return 6; break;
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

void stack_init() {
    while (stack.top != NULL) {
        pop();
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

#ifdef DEBUG
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
#endif

void add_instr(int type, void * ptr1, void * ptr2, void * ptr3) {
    tInstr instr = { type, ptr1, ptr2, ptr3 };

    listInsertLast(&instr_list, instr);

#ifdef DEBUG
    printf("instr: ");
    print_instr(&instr);
#endif
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

    if (temp->symbol != T_L) {
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
        debug_printf("rule: E -> E + E  ");
        add_instr(IN_ADD, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, MINUS, NT_EXPR)) {
        debug_printf("rule: E -> E - E  ");
        add_instr(IN_SUB, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, MUL, NT_EXPR)) {
        debug_printf("rule: E -> E * E  ");
        add_instr(IN_MUL, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, DIV, NT_EXPR)) {
        debug_printf("rule: E -> E / E  ");
        add_instr(IN_DIV, NULL, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, LEFT_BRACKET, NT_EXPR, RIGHT_BRACKET)) {
        debug_printf("rule: E -> (E)    ");
    }
    else if (rule(2, NT_EXPR, ID)) {
        debug_printf("rule: E -> ID     ");
        add_instr(IN_PUSH, (void *) 0x42, NULL, NULL);
    }
    else if (rule(2, NT_EXPR, INT_LITERAL)) {
        debug_printf("rule: E -> INT    ");
        add_instr(IN_PUSH, (void *) 0x01, NULL, NULL);
    }
    else if (rule(2, NT_EXPR, DOUBLE_LITERAL)) {
        debug_printf("rule: E -> DOUBLE ");
        add_instr(IN_PUSH, (void *) 0x02, NULL, NULL);
    }
    else if (rule(2, NT_EXPR, STRING_LITERAL)) {
        debug_printf("rule: E -> STRING ");
        add_instr(IN_PUSH, (void *) 0x02, NULL, NULL);
    }
    else if (rule(4, NT_EXPR, NT_EXPR, LESS, NT_EXPR)) {
        debug_printf("rule: E -> E < E  ");
    }
    else if (rule(4, NT_EXPR, NT_EXPR, GREAT, NT_EXPR)) {
        debug_printf("rule: E -> E > E  ");
    }
    else if (rule(4, NT_EXPR, NT_EXPR, LESS_EQ, NT_EXPR)) {
        debug_printf("rule: E -> E <= E ");
    }
    else if (rule(4, NT_EXPR, NT_EXPR, GREAT_EQ, NT_EXPR)) {
        debug_printf("rule: E -> E >= E ");
    }
    else if (rule(4, NT_EXPR, NT_EXPR, EQUAL, NT_EXPR)) {
        debug_printf("rule: E -> E == E ");
    }
    else if (rule(4, NT_EXPR, NT_EXPR, N_EQUAL, NT_EXPR)) {
        debug_printf("rule: E -> E != E ");
    }
    else {
        debug_printf("rule: no matching rule");
        return SYNTAX_ERROR;
    }

    return SYNTAX_OK;
}

#ifdef DEBUG
void print_symbol(int symbol) {
    debug_print_cnt++;

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
        case LESS:
            printf("'<'");
            debug_print_cnt += 2;
            break;
        case GREAT:
            printf("'>'");
            debug_print_cnt += 2;
            break;
        case LESS_EQ:
            printf("'<='");
            debug_print_cnt += 3;
            break;
        case GREAT_EQ:
            printf("'>='");
            debug_print_cnt += 3;
            break;
        case EQUAL:
            printf("==");
            debug_print_cnt += 1;
            break;
        case N_EQUAL:
            printf("!=");
            debug_print_cnt += 1;
            break;
        case LEFT_BRACKET:
            printf("(");
            break;
        case RIGHT_BRACKET:
            printf(")");
            break;
        case INT_LITERAL:
            printf("int");
            debug_print_cnt += 2;
            break;
        case DOUBLE_LITERAL:
            printf("dbl");
            debug_print_cnt += 2;
            break;
        case STRING_LITERAL:
            printf("str");
            debug_print_cnt += 2;
            break;
        case ID:
            printf("id");
            debug_print_cnt += 1;
            break;
        case END_OF_FILE:
            printf("$");
            break;
        case T_L:
            printf("<");
            break;
        case T_R:
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
}

void print_stack() {
    debug_print_cnt = 0;

    print_stack_item(stack.top);

    for (int i = debug_print_cnt; i < DEBUG_PRINT_STACK_WIDTH; i++)
        printf(" ");
}

void print_symbol_aligned(int symbol) {
    debug_print_cnt = 0;

    print_symbol(symbol);

    for (int i = debug_print_cnt; i < 6; i++)
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
#endif

extern int get_next_token();

int math_expr() {
    int b, result;

    listInit(&instr_list);

    stack_init();

    push(END_OF_FILE);

    b = get_next_token();
    
    do {
#ifdef DEBUG
        printf("stack: ");
        print_stack();
        printf("    input: ");
        print_symbol_aligned(b);
#endif

        switch (table[map_token(top_term())][map_token(b)]) {
            case T_E:
                debug_printf("op: =    ");
                push(b);
                b = get_next_token();
                break;
            case T_L:
                debug_printf("op: <    ");
                insert_after_top_term(T_L);
                push(b);
                b = get_next_token();
                break;
            case T_R:
                debug_printf("op: >    ");
                result = rules();
                if (result == SYNTAX_ERROR) {
                    debug_printf("\n");
                    return SYNTAX_ERROR;
                }
                break;
            case T_N:
            default:
                debug_printf("op: none, ");
                return SYNTAX_ERROR;
                break;
        }

        debug_printf("\n");

    } while (top_term() != END_OF_FILE || b != END_OF_FILE);

#ifdef DEBUG
    printf("stack: ");
    print_stack();
    printf("    input: ");
    print_symbol(b);
    printf("\n");

    printf("\n");
    printf("Generated instructions:\n");
    print_instr_list();
#endif

    return SYNTAX_OK;
}

int bool_expr() {
    return math_expr();
}

