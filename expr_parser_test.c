#include <stdio.h>
#include "expr_parser.h"
#include "parser_test.h"
#include "instrlist.h"

#define INPUT_SIZE 200

typedef struct {
    int expr_type;
    int type;
    char input[INPUT_SIZE];
    int expectedness;
    int tokens[INPUT_SIZE];
} test_case_t;

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define SY SYNTAX_ERROR
#define SE SEMANTIC_ERROR
#define OK SYNTAX_OK

#define NUM_TESTS 150

#define T_BOOL 1
#define T_MATH 2

test_case_t tests[NUM_TESTS] = {
    { T_MATH, TYPE_ERROR,  "<empty> ;",      SY, { SEMICOLON } },
    { T_MATH, TYPE_INT,    "int ;",          OK, { INT_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "dbl ;",          OK, { DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_STRING, "str ;",          OK, { STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i ;",            OK, { ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "int int ;",      SY, { INT_LITERAL, INT_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "dbl dbl ;",      SY, { DOUBLE_LITERAL, DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "str str ;",      SY, { STRING_LITERAL, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "i i ;",          SY, { ID, ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "( ;",            SY, { LEFT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "() ;",           SY, { LEFT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "( i ;",          SY, { LEFT_BRACKET, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "( i ) ;",        OK, { LEFT_BRACKET, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "( int ) ;",      OK, { LEFT_BRACKET, INT_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "( dbl ) ;",      OK, { LEFT_BRACKET, DOUBLE_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_STRING, "( str ) ;",      OK, { LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "( ( i ) ) ;",    OK, { LEFT_BRACKET, LEFT_BRACKET, ID, RIGHT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_STRING, "( ( str ) ) ;",  OK, { LEFT_BRACKET, LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i + i ;",        OK, { ID, PLUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i - i ;",        OK, { ID, MINUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i * i ;",        OK, { ID, MUL, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i / i ;",        OK, { ID, DIV, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i + i + i ;",    OK, { ID, PLUS, ID, PLUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i - i - i ;",    OK, { ID, MINUS, ID, MINUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i * i * i ;",    OK, { ID, MUL, ID, MUL, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i / i / i ;",    OK, { ID, DIV, ID, DIV, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i + i * i ;",    OK, { ID, PLUS, ID, MUL, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i * i - i ;",    OK, { ID, MUL, ID, MINUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i - i / i ;",    OK, { ID, MINUS, ID, DIV, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i / i + i ;",    OK, { ID, DIV, ID, PLUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i + ( i + i ) ;",    OK, { ID, PLUS, LEFT_BRACKET, ID, PLUS, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i - ( i - i ) ;",    OK, { ID, MINUS, LEFT_BRACKET, ID, MINUS, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "( i * i ) * i ;",    OK, { LEFT_BRACKET, ID, MUL, ID, RIGHT_BRACKET, MUL, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "( i / i / i ) ;",    OK, { LEFT_BRACKET, ID, DIV, ID, DIV, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "( i ) + i * i ;",    OK, { LEFT_BRACKET, ID, PLUS, ID, MUL, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i * ( i ) - i ;",    OK, { ID, MUL, LEFT_BRACKET, ID, RIGHT_BRACKET, MINUS, ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i - i / ( i ) ;",    OK, { ID, MINUS, ID, DIV, LEFT_BRACKET, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "i ( i + i ) ;",  SY, { ID, LEFT_BRACKET, ID, PLUS, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "( i * i ) i ;",  SY, { LEFT_BRACKET, ID, MUL, ID, RIGHT_BRACKET, ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "i + ;",          SY, { ID, PLUS, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "i - ;",          SY, { ID, MINUS, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "i * ;",          SY, { ID, MUL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "i / ;",          SY, { ID, DIV, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "+ i ;",          SY, { PLUS, ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "- i ;",          SY, { MINUS, ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "* i ;",          SY, { MUL, ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "/ i ;",          SY, { DIV, ID, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "+ ;",            SY, { PLUS, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "- ;",            SY, { MINUS, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "* ;",            SY, { MUL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "/ ;",            SY, { DIV, SEMICOLON } },

    { T_MATH, TYPE_DOUBLE, "int + dbl ;",    OK, { INT_LITERAL, PLUS, DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "dbl + int ;",    OK, { DOUBLE_LITERAL, PLUS, INT_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "int - dbl ;",    OK, { INT_LITERAL, MINUS, DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "dbl - int ;",    OK, { DOUBLE_LITERAL, MINUS, INT_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "int * dbl ;",    OK, { INT_LITERAL, MUL, DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "dbl * int ;",    OK, { DOUBLE_LITERAL, MUL, INT_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "int / dbl ;",    OK, { INT_LITERAL, DIV, DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_DOUBLE, "dbl / int ;",    OK, { DOUBLE_LITERAL, DIV, INT_LITERAL, SEMICOLON } },

    { T_MATH, TYPE_ERROR,  "i + str ;",      SE, { ID, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "int + str ;",    SE, { INT_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "dbl + str ;",    SE, { DOUBLE_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "str - str ;",    SE, { STRING_LITERAL, MINUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "str * str ;",    SE, { STRING_LITERAL, MUL, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "str / str ;",    SE, { STRING_LITERAL, DIV, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_STRING, "str + str ;",    OK, { STRING_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_STRING, "str + str + str ;",      OK, { STRING_LITERAL, PLUS, STRING_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, TYPE_STRING, "str + ( str ) ;",        OK, { STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_STRING, "str + ( str + str ) ;",  OK, { STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, PLUS, STRING_LITERAL, RIGHT_BRACKET, SEMICOLON } },

    { T_BOOL, TYPE_BOOL,   "i < i )",        OK, { ID, LESS, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "i > i )",        OK, { ID, GREAT, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "i <= i )",       OK, { ID, LESS_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "i >= i )",       OK, { ID, GREAT_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "i == i )",       OK, { ID, EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "i != i )",       OK, { ID, N_EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i < )",          SY, { ID, LESS, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i > )",          SY, { ID, GREAT, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i <= )",         SY, { ID, LESS_EQ, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i >= )",         SY, { ID, GREAT_EQ, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i == )",         SY, { ID, EQUAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i != )",         SY, { ID, N_EQUAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "< i )",          SY, { LESS, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "> i )",          SY, { GREAT, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "<= i )",         SY, { LESS_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  ">= i )",         SY, { GREAT_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "== i )",         SY, { EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "!= i )",         SY, { N_EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i < i < i )",    SY, { ID, LESS, ID, LESS, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i > i > i )",    SY, { ID, GREAT, ID, GREAT, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i <= i <= i )",  SY, { ID, LESS_EQ, ID, LESS_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i >= i >= i )",  SY, { ID, GREAT_EQ, ID, GREAT_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i == i == i )",  SY, { ID, EQUAL, ID, EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i != i != i )",  SY, { ID, N_EQUAL, ID, N_EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "str < str )",    SE, { STRING_LITERAL, LESS, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "str > str )",    SE, { STRING_LITERAL, GREAT, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "str <= str )",   SE, { STRING_LITERAL, LESS_EQ, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "str >= str )",   SE, { STRING_LITERAL, GREAT_EQ, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "str == str )",   SE, { STRING_LITERAL, EQUAL, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "str != str )",   SE, { STRING_LITERAL, N_EQUAL, STRING_LITERAL, RIGHT_BRACKET } },

    { T_BOOL, TYPE_BOOL,   "int < dbl )",    OK, { INT_LITERAL, LESS, DOUBLE_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "dbl > int )",    OK, { DOUBLE_LITERAL, GREAT, INT_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "int <= dbl )",   OK, { INT_LITERAL, LESS_EQ, DOUBLE_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "dbl >= int )",   OK, { DOUBLE_LITERAL, GREAT_EQ, INT_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "int == dbl )",   OK, { INT_LITERAL, EQUAL, DOUBLE_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "dbl != int )",   OK, { DOUBLE_LITERAL, N_EQUAL, INT_LITERAL, RIGHT_BRACKET } },

    { T_BOOL, TYPE_ERROR,  ")",              SE, { RIGHT_BRACKET } },
    { T_MATH, TYPE_ERROR,  ";",              SY, { SEMICOLON } },
    { T_BOOL, TYPE_ERROR,  "i )",            SE, { ID, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "int > int )",    OK, { INT_LITERAL, GREAT, INT_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, TYPE_ERROR,  "i ) )",          SE, { ID, RIGHT_BRACKET, RIGHT_BRACKET } },
    { T_MATH, TYPE_INT,    "i ;",            OK, { ID, SEMICOLON } },
    { T_MATH, TYPE_INT,    "i ; ;",          OK, { ID, SEMICOLON, SEMICOLON } },
    { T_BOOL, TYPE_ERROR,  "( i ) )",        SE, { LEFT_BRACKET, ID, RIGHT_BRACKET, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL,   "( int > int ) )",OK, { LEFT_BRACKET, INT_LITERAL, GREAT, INT_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET } },
    { T_MATH, TYPE_INT,    "( i ) ;",        OK, { LEFT_BRACKET, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, TYPE_ERROR,  "( i ;",          SY, { LEFT_BRACKET, ID, SEMICOLON } },

    { T_BOOL, TYPE_BOOL, "i < int + ((i * (dbl - (i / i)))) ;", OK, { ID, LESS, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, ID, MINUS, LEFT_BRACKET, ID, DIV, ID, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET } },
    { T_BOOL, TYPE_BOOL, "i != int + ((dbl * (int - i) / int)) ;", OK, { ID, N_EQUAL, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, INT_LITERAL, MINUS, ID, RIGHT_BRACKET, DIV, INT_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET } },
};

int test_tokens_counter = 0;
int test_number = 0;

int get_next_token() {
    int result = tests[test_number].tokens[test_tokens_counter];

    if (result != END_OF_FILE && test_tokens_counter < INPUT_SIZE) {
        test_tokens_counter++;
    }

    return result;
}

void return_token(int symbol) {
    printf("returning token: %d", symbol);
}

int main(int __attribute__((unused)) argc, char __attribute__((unused)) **argv) {
    printf("**************************************************************************\n");
    printf(" Expression parser test\n");
    printf("**************************************************************************\n\n");

    int exit_code = 0;
    int i, result, type;

    for (i = 0; i < NUM_TESTS; i++) {
        if (tests[i].input[0] == 0) {
            break;
        }

        printf(COLOR_YELLOW);
        printf("**************************************************************************\n");
        printf(" Test n. %d\n", i + 1);
        printf(" input: '%s'\n", tests[i].input);
        
        if (tests[i].expr_type == T_BOOL)
            printf(" type: BOOL\n");
        else if (tests[i].expr_type == T_MATH)
            printf(" type: MATH\n");

        printf("**************************************************************************\n\n");
        printf(COLOR_RESET);

        test_tokens_counter = 0;
        test_number = i;

        if (tests[i].expr_type == T_BOOL) {
            result = bool_expr();

            if (result == SYNTAX_OK)
                type = TYPE_BOOL;
            else
                type = TYPE_ERROR;
        }
        else if (tests[i].expr_type == T_MATH)
            result = math_expr(&type);
        else
            result = -1;

        printf(COLOR_BLUE);
        printf("\n**************************************************************************\n");

        switch (result) {
            case SYNTAX_OK:
                printf(" parser result: SYNTAX_OK\n"); break;
            case SYNTAX_ERROR:
                printf(" parser result: SYNTAX_ERROR\n"); break;
            case SEMANTIC_ERROR:
                printf(" parser result: SEMANTIC_ERROR\n"); break;
            default:
                printf(" parser result: other\n");
        }

        switch (type) {
            case TYPE_NONE:
                printf(" result type: none\n"); break;
            case TYPE_ERROR:
                printf(" result type: error\n"); break;
            case TYPE_VOID:
                printf(" result type: void\n"); break;
            case TYPE_INT:
                printf(" result type: int\n"); break;
            case TYPE_DOUBLE:
                printf(" result type: double\n"); break;
            case TYPE_STRING:
                printf(" result type: string\n"); break;
            case TYPE_BOOL:
                printf(" result type: bool\n"); break;
            default:
                printf(" result_type: other\n");
        }

        if (result == tests[i].expectedness
            && type == tests[i].type) {
            printf(" test result: " COLOR_GREEN "PASS\n" COLOR_BLUE);
        }
        else {
            printf(" test result: " COLOR_RED "FAIL\n" COLOR_BLUE);
            exit_code = -1;
        }

        printf("**************************************************************************\n\n");
        printf(COLOR_RESET);
    }

    printf(COLOR_CYAN);
    printf("**************************************************************************\n");
    printf(" Total %d tests, ", i);

    if (exit_code == 0)
        printf(" test result: " COLOR_GREEN "PASS\n" COLOR_CYAN);
    else
        printf(" test result: " COLOR_RED "FAIL\n" COLOR_CYAN);

    printf("**************************************************************************\n\n");
    printf(COLOR_RESET);

    return exit_code;
}
