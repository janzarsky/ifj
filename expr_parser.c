#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "parser_test.h"
#include "instrlist.h"
#include "expr_parser.h"

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
    int type;
    struct stack_item *next;
} stack_item_t;

typedef struct stack{
    stack_item_t *top;
} stack_t;

stack_t stack = { NULL };

tListOfInstr instr_list;

// debugging functions
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
        case IN_FADD:
            printf("FADD"); break;
        case IN_FSUB:
            printf("FSUB"); break;
        case IN_FMUL:
            printf("FMUL"); break;
        case IN_FDIV:
            printf("FDIV"); break;
        case IN_PUSH:
            printf("PUSH"); break;
        case IN_CONV:
            printf("CONV"); break;
        case IN_SWAP:
            printf("SWAP"); break;
        case IN_CONCAT:
            printf("CONCAT"); break;
        default:
            printf("%d", instr->instType);
    }

    printf(" %p %p %p, ", instr->addr1, instr->addr2, instr->addr3);
}

void print_type(int type) {
    switch (type) {
        case TYPE_NONE:
            printf("none   "); break;
        case TYPE_ERROR:
            printf("error  "); break;
        case TYPE_VOID:
            printf("void   "); break;
        case TYPE_INT:
            printf("int    "); break;
        case TYPE_DOUBLE:
            printf("double "); break;
        case TYPE_STRING:
            printf("string "); break;
        case TYPE_BOOL:
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

void add_instr(int type, void * ptr1, void * ptr2, void * ptr3) {
    tInstr instr = { type, ptr1, ptr2, ptr3 };

    listInsertLast(&instr_list, instr);

#ifdef DEBUG
    printf("instr: ");
    print_instr(&instr);
#endif
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
    if (type == TYPE_ERROR)
        return SEMANTIC_ERROR;

    // pop additional '<' from stack
    pop_n_times(num + 1);

#ifdef DEBUG
    debug_printf("pushing type: ");
    print_type(type);
    debug_printf("  ");
#endif

    push(symbol, type);

    return SYNTAX_OK;
}

int check_type_arithmetic() {
    int type1 = stack.top->type;
    int type2 = stack.top->next->next->type;

    if ((type1 == TYPE_INT) && (type2 == TYPE_INT))
        return TYPE_INT;

    if ((type1 == TYPE_DOUBLE) && (type2 == TYPE_DOUBLE))
        return TYPE_DOUBLE;

    if ((type1 == TYPE_INT) && (type2 == TYPE_DOUBLE)) {
        add_instr(IN_CONV, NULL, NULL, NULL);
        return TYPE_DOUBLE;
    }

    if ((type1 == TYPE_DOUBLE) && (type2 == TYPE_INT)) {
        add_instr(IN_SWAP, NULL, NULL, NULL);
        add_instr(IN_CONV, NULL, NULL, NULL);
        add_instr(IN_SWAP, NULL, NULL, NULL);
        return TYPE_DOUBLE;
    }

    return TYPE_ERROR;
}

int check_type_add() {
    int type = check_type_arithmetic();

    if (type == TYPE_INT) {
        add_instr(IN_ADD, NULL, NULL, NULL);
    }
    else if (type == TYPE_DOUBLE) {
        add_instr(IN_FADD, NULL, NULL, NULL);
    }
    else if ((stack.top->type == TYPE_STRING) &&
             (stack.top->next->next->type == TYPE_STRING)) {
        type = TYPE_STRING;
        add_instr(IN_CONCAT, NULL, NULL, NULL);
    }

    return type;
}

int check_type_minus() {
    int type = check_type_arithmetic();

    if (type == TYPE_INT)
        add_instr(IN_SUB, NULL, NULL, NULL);
    else if (type == TYPE_DOUBLE)
        add_instr(IN_FSUB, NULL, NULL, NULL);

    return type;
}

int check_type_mul() {
    int type = check_type_arithmetic();

    if (type == TYPE_INT)
        add_instr(IN_MUL, NULL, NULL, NULL);
    else if (type == TYPE_DOUBLE)
        add_instr(IN_FMUL, NULL, NULL, NULL);

    return type;
}

int check_type_div() {
    int type = check_type_arithmetic();

    if (type == TYPE_INT)
        add_instr(IN_DIV, NULL, NULL, NULL);
    else if (type == TYPE_DOUBLE)
        add_instr(IN_FDIV, NULL, NULL, NULL);

    return type;
}

int check_type_brackets() {
    return stack.top->next->type;
}

int check_type_id() {
    // TODO get type from table of symbols
    return TYPE_INT;
}

int check_type_rel() {
    int type1 = stack.top->type;
    int type2 = stack.top->next->next->type;

    if (((type1 == TYPE_INT) || (type1 == TYPE_DOUBLE))
        && ((type2 == TYPE_INT) || (type2 == TYPE_DOUBLE)))
        return TYPE_BOOL;
    else
        return TYPE_ERROR;
}

int rules() {
    int result, type;

    if (check_rule(3, NT_EXPR, PLUS, NT_EXPR)) {
        debug_printf("rule: E -> E + E  ");
        type = check_type_add();
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, MINUS, NT_EXPR)) {
        debug_printf("rule: E -> E - E  ");
        type = check_type_minus();
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, MUL, NT_EXPR)) {
        debug_printf("rule: E -> E * E  ");
        type = check_type_mul();
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, NT_EXPR, DIV, NT_EXPR)) {
        debug_printf("rule: E -> E / E  ");
        type = check_type_div();
        result = execute_rule(3, NT_EXPR, type);
    }
    else if (check_rule(3, LEFT_BRACKET, NT_EXPR, RIGHT_BRACKET)) {
        type = check_type_brackets();
        result = execute_rule(3, NT_EXPR, type);
        debug_printf("rule: E -> (E)    ");
    }
    else if (check_rule(1, ID)) {
        type = check_type_id();
        result = execute_rule(1, NT_EXPR, type);
        debug_printf("rule: E -> ID     ");
        add_instr(IN_PUSH, (void *) 0x42, NULL, NULL);
    }
    else if (check_rule(1, INT_LITERAL)) {
        result = execute_rule(1, NT_EXPR, TYPE_INT);
        debug_printf("rule: E -> INT    ");
        add_instr(IN_PUSH, (void *) 0x01, NULL, NULL);
    }
    else if (check_rule(1, DOUBLE_LITERAL)) {
        result = execute_rule(1, NT_EXPR, TYPE_DOUBLE);
        debug_printf("rule: E -> DOUBLE ");
        add_instr(IN_PUSH, (void *) 0x02, NULL, NULL);
    }
    else if (check_rule(1, STRING_LITERAL)) {
        result = execute_rule(1, NT_EXPR, TYPE_STRING);
        debug_printf("rule: E -> STRING ");
        add_instr(IN_PUSH, (void *) 0x02, NULL, NULL);
    }
    else if (check_rule(3, NT_EXPR, LESS, NT_EXPR)) {
        type = check_type_rel();
        result = execute_rule(3, NT_EXPR, check_type_rel());
        debug_printf("rule: E -> E < E  ");
    }
    else if (check_rule(3, NT_EXPR, GREAT, NT_EXPR)) {
        type = check_type_rel();
        result = execute_rule(3, NT_EXPR, check_type_rel());
        debug_printf("rule: E -> E > E  ");
    }
    else if (check_rule(3, NT_EXPR, LESS_EQ, NT_EXPR)) {
        type = check_type_rel();
        result = execute_rule(3, NT_EXPR, check_type_rel());
        debug_printf("rule: E -> E <= E ");
    }
    else if (check_rule(3, NT_EXPR, GREAT_EQ, NT_EXPR)) {
        type = check_type_rel();
        result = execute_rule(3, NT_EXPR, check_type_rel());
        debug_printf("rule: E -> E >= E ");
    }
    else if (check_rule(3, NT_EXPR, EQUAL, NT_EXPR)) {
        type = check_type_rel();
        result = execute_rule(3, NT_EXPR, check_type_rel());
        debug_printf("rule: E -> E == E ");
    }
    else if (check_rule(3,  NT_EXPR, N_EQUAL, NT_EXPR)) {
        type = check_type_rel();
        result = execute_rule(3, NT_EXPR, check_type_rel());
        debug_printf("rule: E -> E != E ");
    }
    else {
        debug_printf("rule: no matching rule");
        return SYNTAX_ERROR;
    }

    if (result == SEMANTIC_ERROR)
        return SEMANTIC_ERROR;

    return SYNTAX_OK;
}


extern int get_next_token();

#define MATH_EXPR 1
#define BOOL_EXPR 2

int expr(int expr_type, int *type) {
    int b, result;

    listInit(&instr_list);

    stack_init();

    push(END_OF_FILE, TYPE_NONE);

    b = get_next_token();
    
    do {
#ifdef DEBUG
        printf("stack: ");
        print_stack();
        printf("    input: ");
        print_symbol_aligned(b);
#endif
        if (expr_type == MATH_EXPR && b == SEMICOLON) {
            debug_printf(", ");
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
                push(b, TYPE_NONE);
                b = get_next_token();
                break;
            case T_L:
                debug_printf("op: <    ");
                insert_after_top_term(T_L);
                push(b, TYPE_NONE);
                b = get_next_token();
                break;
            case T_R:
                debug_printf("op: >    ");
                result = rules();
                if (result == SYNTAX_ERROR) {
                    debug_printf("\n");
                    *type = TYPE_ERROR;
                    return SYNTAX_ERROR;
                }
                else if (result == SEMANTIC_ERROR) {
                    debug_printf("\n");
                    *type = TYPE_ERROR;
                    return SEMANTIC_ERROR;
                }
                break;
            case T_N:
            default:
                debug_printf("op: none, \n");
                *type = TYPE_ERROR;
                return SYNTAX_ERROR;
        }

        debug_printf("\n");

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
        if ((*type == TYPE_INT) || 
            (*type == TYPE_DOUBLE) ||
            (*type == TYPE_STRING)) {
            return SYNTAX_OK;
        }
        else {
            *type = TYPE_ERROR;
            return SEMANTIC_ERROR;
        }
    }

    if (expr_type == BOOL_EXPR) {
        if (*type == TYPE_BOOL) {
            return SYNTAX_OK;
        }
        else {
            *type = TYPE_ERROR;
            return SEMANTIC_ERROR;
        }
    }

    return SYNTAX_ERROR;
}

int bool_expr(int *type) {
    return expr(BOOL_EXPR, type);
}

int math_expr(int *type) {
    return expr(MATH_EXPR, type);
}
