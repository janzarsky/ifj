#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <string.h>
#include "parser_test.h"
#include "instrlist.h"
#include "expr_parser.h"
#include "scanner.h"
#include "symtab.h"
#include "debug.h"
#include "error_codes.h"

extern symtab_t *tabulka;
extern symtab_t *local_tabulka;
extern int token;
extern char *token_data;

extern tListOfInstr instr_list;

char *token_data_prev;

#ifdef DEBUG

#define DEBUG_PRINT_STACK_WIDTH 40
int debug_print_cnt = 0;

#endif

enum nonterm_t { NT_EXPR = TOKEN_MAX, NT_MAX };
enum table_entry_t { T_N = NT_MAX, T_L, T_E, T_R, T_MAX }; // none, <, =, >

#define T_SIZE 7

const char table[T_SIZE][T_SIZE] = {
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
    int type;
    struct stack_item *next;
} stack_item_t;

typedef struct stack{
    stack_item_t *top;
} stack_t;

stack_t stack = { NULL };

// debugging functions
#ifdef DEBUG
void print_type(int type) {
    switch (type) {
        case ST_DATATYPE_ERROR:
            printf("error  "); break;
        case ST_DATATYPE_VOID:
            printf("void   "); break;
        case ST_DATATYPE_INT:
            printf("int    "); break;
        case ST_DATATYPE_DOUBLE:
            printf("double "); break;
        case ST_DATATYPE_STRING:
            printf("string "); break;
        case ST_DATATYPE_BOOL:
            printf("bool   "); break;
        default:
            printf("other  ");
    }
}

void print_symbol(int symbol) {
    debug_print_cnt++;

    switch (symbol) {
        case PLUS:
            printf("+"); break;
        case MINUS:
            printf("-"); break;
        case MUL:
            printf("*"); break;
        case DIV:
            printf("/"); break;
        case LESS:
            printf("'<'"); debug_print_cnt += 2; break;
        case GREAT:
            printf("'>'"); debug_print_cnt += 2; break;
        case LESS_EQ:
            printf("'<='"); debug_print_cnt += 3; break;
        case GREAT_EQ:
            printf("'>='"); debug_print_cnt += 3; break;
        case EQUAL:
            printf("=="); debug_print_cnt += 1; break;
        case N_EQUAL:
            printf("!="); debug_print_cnt += 1; break;
        case LEFT_BRACKET:
            printf("("); break;
        case RIGHT_BRACKET:
            printf(")"); break;
        case INT_LITERAL:
            printf("int"); debug_print_cnt += 2; break;
        case DOUBLE_LITERAL:
            printf("dbl"); debug_print_cnt += 2; break;
        case STRING_LITERAL:
            printf("str"); debug_print_cnt += 2; break;
        case ID:
            printf("id"); debug_print_cnt += 1; break;
        case SEMICOLON:
            printf(";"); break;
        case END_OF_FILE:
            printf("$"); break;
        case T_L:
            printf("<"); break;
        case T_R:
            printf(">"); break;
        case NT_EXPR:
            printf("E"); break;
        default:
            printf("%d", symbol); break;
    }
}

void print_stack_item(stack_item_t *item) {
    if (item->next != NULL)
        print_stack_item(item->next);

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
#endif

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

void push(int symbol, int type) {
    stack_item_t *temp = malloc(sizeof(stack_item_t));

    if (temp == NULL)
        return;

    temp->next = stack.top;
    temp->symbol = symbol;
    temp->type = type;

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
    if (stack.top != NULL)
        return stack.top->symbol;
    else
        return END_OF_FILE;
}

int is_term(int symbol) {
    return (symbol < TOKEN_MAX);
}

int top_term() {
    stack_item_t *temp = stack.top;

    while (temp != NULL) {
        if (is_term(temp->symbol))
            return temp->symbol;
        else
            temp = temp->next;
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
            
            if (prev == NULL)
                stack.top = new_item;
            else
                prev->next = new_item;

            return;
        }
        else {
            prev = temp;
            temp = temp->next;
        }
    }
}

bool check_rule(int num, ...) {
    va_list valist;
    va_start(valist, num);

    // copy arguments (we need them in reverse order)
    int *symbols = malloc(num*sizeof(int));

    if (symbols == NULL) {
        va_end(valist);
        return false;
    }

    for (int i = 0; i < num; i++)
        symbols[num - 1 - i] = va_arg(valist, int);
    
    // compare stack with arguments
    stack_item_t *temp = stack.top;

    for (int i = 0; i < num; i++) {
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

    va_end(valist);
    return true;
}

int execute_rule(int num, int symbol, int type) {
    if (type == ST_DATATYPE_ERROR)
        return ER_SEM;

    // pop additional '<' from stack
    pop_n_times(num + 1);

#ifdef DEBUG
    debug_printf("pushing type: ");
    print_type(type);
    debug_printf("  ");
#endif

    push(symbol, type);

    return ER_OK;
}

int check_and_convert_numeric_types() {
    int type1 = stack.top->type;
    int type2 = stack.top->next->next->type;

    if ((type1 == ST_DATATYPE_INT) && (type2 == ST_DATATYPE_INT))
        return ST_DATATYPE_INT;

    if ((type1 == ST_DATATYPE_DOUBLE) && (type2 == ST_DATATYPE_DOUBLE))
        return ST_DATATYPE_DOUBLE;

    if ((type1 == ST_DATATYPE_INT) && (type2 == ST_DATATYPE_DOUBLE)) {
        add_instr(IN_CONV, NULL, NULL, NULL);
        return ST_DATATYPE_DOUBLE;
    }

    if ((type1 == ST_DATATYPE_DOUBLE) && (type2 == ST_DATATYPE_INT)) {
        add_instr(IN_SWAP, NULL, NULL, NULL);
        add_instr(IN_CONV, NULL, NULL, NULL);
        add_instr(IN_SWAP, NULL, NULL, NULL);
        return ST_DATATYPE_DOUBLE;
    }

    return ST_DATATYPE_ERROR;
}

int check_type_arithmetic(int instr) {
    int type = check_and_convert_numeric_types();

    if (type == ST_DATATYPE_INT) {
        add_instr(instr, NULL, NULL, NULL);
    }
    else if (type == ST_DATATYPE_DOUBLE) {
        add_instr(instr + F_ARITH_OFFSET, NULL, NULL, NULL);
    }
    else if (instr == IN_ADD &&
             (stack.top->type == ST_DATATYPE_STRING) &&
             (stack.top->next->next->type == ST_DATATYPE_STRING)) {
        type = ST_DATATYPE_STRING;
        add_instr(IN_CONCAT, NULL, NULL, NULL);
    }

    return type;
}

int check_type_brackets() {
    return stack.top->next->type;
}

int check_type_id() {
    symtab_elem_t *var = st_find(local_tabulka, token_data_prev);

    if (var == NULL) {
        var = st_find(tabulka, token_data_prev);

        if (var == NULL)
            return ST_DATATYPE_ERROR;
    }

    if (var->elem_type == ST_ELEMTYPE_VAR)
        return var->data_type;

    return ST_DATATYPE_ERROR;
}

int check_type_rel(int instr) {
    int type = check_and_convert_numeric_types();

    if (type == ST_DATATYPE_INT) {
        type = ST_DATATYPE_BOOL;
        add_instr(instr, NULL, NULL, NULL);
    }
    else if (type == ST_DATATYPE_DOUBLE) {
        type = ST_DATATYPE_BOOL;
        add_instr(instr + F_REL_OFFSET, NULL, NULL, NULL);
    }
    
    return type;
}


long int int_from_token(char *token_data) {
    return strtol(token_data, NULL, 10);
}

double *double_from_token(char *token_data) {
    double d = strtod(token_data, NULL);

    double *val = malloc(sizeof(double));

    if (val == NULL)
        return NULL;

    *val = d;

    return val;
}

char *string_from_token(char *token_data) {
    return strdup(token_data);
}

int rules() {
    int result, type;

    if (check_rule(3, NT_EXPR, PLUS, NT_EXPR)) {
        debug_printf("rule: E -> E + E  ");
        type = check_type_arithmetic(IN_ADD);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, MINUS, NT_EXPR)) {
        debug_printf("rule: E -> E - E  ");
        type = check_type_arithmetic(IN_SUB);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, MUL, NT_EXPR)) {
        debug_printf("rule: E -> E * E  ");
        type = check_type_arithmetic(IN_MUL);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, DIV, NT_EXPR)) {
        debug_printf("rule: E -> E / E  ");
        type = check_type_arithmetic(IN_DIV);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, LEFT_BRACKET, NT_EXPR, RIGHT_BRACKET)) {
        debug_printf("rule: E -> (E)    ");
        type = check_type_brackets();
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(1, ID)) {
        debug_printf("rule: E -> ID     ");
        type = check_type_id();
        result = execute_rule(1, NT_EXPR, type);

        debug_printf("token_data_prev: %s ", token_data_prev);

        symtab_elem_t *var = st_find(tabulka, token_data_prev);

        if (var == NULL) {
            var = st_find(local_tabulka, token_data_prev);

            if (var == NULL)
                result = ER_SEM;
        }

        add_instr(IN_TAB_PUSH, (void *) var, NULL, NULL);
    }
    else if (check_rule(1, INT_LITERAL)) {
        debug_printf("rule: E -> INT    ");
        result = execute_rule(1, NT_EXPR, ST_DATATYPE_INT);
        long int value = int_from_token(token_data_prev);
        add_instr(IN_VAL_PUSH, (void *) value, NULL, NULL);
    }
    else if (check_rule(1, DOUBLE_LITERAL)) {
        debug_printf("rule: E -> DOUBLE ");
        result = execute_rule(1, NT_EXPR, ST_DATATYPE_DOUBLE);

        double *value = double_from_token(token_data_prev);

        if (value == NULL)
            return ER_SEM;

        add_instr(IN_VAL_PUSH, (void *) value, NULL, NULL);
    }
    else if (check_rule(1, STRING_LITERAL)) {
        debug_printf("rule: E -> STRING ");
        result = execute_rule(1, NT_EXPR, ST_DATATYPE_STRING);

        char *value = string_from_token(token_data_prev);

        if (value == NULL)
            return ER_SEM;

        add_instr(IN_VAL_PUSH, (void *) value, NULL, NULL);
    }
    else if (check_rule(3, NT_EXPR, LESS, NT_EXPR)) {
        debug_printf("rule: E -> E < E  ");
        type = check_type_rel(IN_LESS);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, GREAT, NT_EXPR)) {
        debug_printf("rule: E -> E > E  ");
        type = check_type_rel(IN_GREAT);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, LESS_EQ, NT_EXPR)) {
        debug_printf("rule: E -> E <= E ");
        type = check_type_rel(IN_LESS_EQ);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, GREAT_EQ, NT_EXPR)) {
        debug_printf("rule: E -> E >= E ");
        type = check_type_rel(IN_GREAT_EQ);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, EQUAL, NT_EXPR)) {
        debug_printf("rule: E -> E == E ");
        type = check_type_rel(IN_EQ);
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3,  NT_EXPR, N_EQUAL, NT_EXPR)) {
        debug_printf("rule: E -> E != E ");
        type = check_type_rel(IN_N_EQ);
        result = execute_rule(3, NT_EXPR, type);
    }
    else {
        debug_printf("rule: no matching rule");
        return ER_SYNTAX;
    }

    if (result == ER_SEM)
        return ER_SEM;

    return ER_OK;
}


#define MATH_EXPR 1
#define BOOL_EXPR 2

int expr(int expr_type, int *type) {
    int b, result;

    stack_init();

    push(END_OF_FILE, ST_DATATYPE_VOID);

    b = get_next_token(&token_data);
    
    do {
#ifdef DEBUG
        printf("stack: ");
        print_stack();
        printf("    input: ");
        print_symbol_aligned(b);
#endif
        if (expr_type == MATH_EXPR && b == SEMICOLON) {
            b = END_OF_FILE;
        }
        else if (expr_type == BOOL_EXPR
            && b == RIGHT_BRACKET && top_term() == END_OF_FILE) {
            debug_printf("\n");
            break;
        }

        switch (table[map_token(top_term())][map_token(b)]) {
            case T_E:
                debug_printf("op: =    ");
                push(b, ST_DATATYPE_VOID);
                token_data_prev = token_data;
                b = get_next_token(&token_data);
                break;
            case T_L:
                debug_printf("op: <    ");
                insert_after_top_term(T_L);
                push(b, ST_DATATYPE_VOID);
                token_data_prev = token_data;
                b = get_next_token(&token_data);
                break;
            case T_R:
                debug_printf("op: >    ");
                result = rules();
                if (result == ER_SYNTAX) {
                    debug_printf("\n");
                    *type = ST_DATATYPE_ERROR;
                    return ER_SYNTAX;
                }
                else if (result == ER_SEM) {
                    debug_printf("\n");
                    *type = ST_DATATYPE_ERROR;
                    return ER_SEM;
                }
                debug_printf("\n");
                break;
            case T_N:
            default:
                debug_printf("op: none, \n");
                *type = ST_DATATYPE_ERROR;
                return ER_SYNTAX;
        }

    } while (top_term() != END_OF_FILE || b != END_OF_FILE);

#ifdef DEBUG
    printf("********** END OF ALGORITM **********\n");
    printf("stack: ");
    print_stack();
    printf("    input: ");
    print_symbol(b);
    printf("\n");

    printf("\n");
    printf("Generated instructions:\n");
    print_instr_list();
#endif

    *type = stack.top->type;

    if (expr_type == MATH_EXPR) {
        if ((*type == ST_DATATYPE_INT) || 
            (*type == ST_DATATYPE_DOUBLE) ||
            (*type == ST_DATATYPE_STRING)) {
            return ER_OK;
        }
        else {
            *type = ST_DATATYPE_ERROR;
            return ER_SEM;
        }
    }

    if (expr_type == BOOL_EXPR) {
        if (*type == ST_DATATYPE_BOOL) {
            return ER_OK;
        }
        else {
            *type = ST_DATATYPE_ERROR;
            return ER_SEM;
        }
    }

    return ER_SYNTAX;
}

int bool_expr() {
    int type;

    debug_printf("*****\nEXPR_PARSER: function bool_expr()\n");

    int result = expr(BOOL_EXPR, &type);

    debug_printf("*****\n");

    return result;
}

int math_expr(int *type) {
    debug_printf("*****\nEXPR_PARSER: function math_expr()\n");

    int result = expr(MATH_EXPR, type);

    debug_printf("*****\n");
    
    return result;
}
