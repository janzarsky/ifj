#include <stdio.h>
#include "expr_parser.h"
#include "parser_test.h"
#include "instrlist.h"

#define INPUT_SIZE 200

typedef struct {
    int expr_type;
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

#define ER SYNTAX_ERROR
#define OK SYNTAX_OK

#define NUM_TESTS 120

#define T_BOOL 1
#define T_MATH 2

test_case_t tests[NUM_TESTS] = {
    { T_MATH, "<empty> ;",      ER, { SEMICOLON } },
    { T_MATH, "int ;",          OK, { INT_LITERAL, SEMICOLON } },
    { T_MATH, "double ;",       OK, { DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, "string ;",       OK, { STRING_LITERAL, SEMICOLON } },
    { T_MATH, "i ;",            OK, { ID, SEMICOLON } },
    { T_MATH, "int int ;",      ER, { INT_LITERAL, INT_LITERAL, SEMICOLON } },
    { T_MATH, "dbl dbl ;",      ER, { DOUBLE_LITERAL, DOUBLE_LITERAL, SEMICOLON } },
    { T_MATH, "str str ;",      ER, { STRING_LITERAL, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "i i ;",          ER, { ID, ID, SEMICOLON } },
    { T_MATH, "( ;",            ER, { LEFT_BRACKET, SEMICOLON } },
    { T_MATH, "() ;",           ER, { LEFT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( i ;",          ER, { LEFT_BRACKET, ID, SEMICOLON } },
    { T_MATH, "( i ) ;",        OK, { LEFT_BRACKET, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( int ) ;",      OK, { LEFT_BRACKET, INT_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( dbl ) ;",      OK, { LEFT_BRACKET, DOUBLE_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( str ) ;",      OK, { LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( ( i ) ) ;",    OK, { LEFT_BRACKET, LEFT_BRACKET, ID, RIGHT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( ( str ) ) ;",  OK, { LEFT_BRACKET, LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "i + i ;",        OK, { ID, PLUS, ID, SEMICOLON } },
    { T_MATH, "i - i ;",        OK, { ID, MINUS, ID, SEMICOLON } },
    { T_MATH, "i * i ;",        OK, { ID, MUL, ID, SEMICOLON } },
    { T_MATH, "i / i ;",        OK, { ID, DIV, ID, SEMICOLON } },
    { T_MATH, "i + i + i ;",    OK, { ID, PLUS, ID, PLUS, ID, SEMICOLON } },
    { T_MATH, "i - i - i ;",    OK, { ID, MINUS, ID, MINUS, ID, SEMICOLON } },
    { T_MATH, "i * i * i ;",    OK, { ID, MUL, ID, MUL, ID, SEMICOLON } },
    { T_MATH, "i / i / i ;",    OK, { ID, DIV, ID, DIV, ID, SEMICOLON } },
    { T_MATH, "i + i * i ;",    OK, { ID, PLUS, ID, MUL, ID, SEMICOLON } },
    { T_MATH, "i * i - i ;",    OK, { ID, MUL, ID, MINUS, ID, SEMICOLON } },
    { T_MATH, "i - i / i ;",    OK, { ID, MINUS, ID, DIV, ID, SEMICOLON } },
    { T_MATH, "i / i + i ;",    OK, { ID, DIV, ID, PLUS, ID, SEMICOLON } },
    { T_MATH, "i + ( i + i ) ;",    OK, { ID, PLUS, LEFT_BRACKET, ID, PLUS, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "i - ( i - i ) ;",    OK, { ID, MINUS, LEFT_BRACKET, ID, MINUS, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( i * i ) * i ;",    OK, { LEFT_BRACKET, ID, MUL, ID, RIGHT_BRACKET, MUL, ID, SEMICOLON } },
    { T_MATH, "( i / i / i ) ;",    OK, { LEFT_BRACKET, ID, DIV, ID, DIV, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( i ) + i * i ;",    OK, { LEFT_BRACKET, ID, PLUS, ID, MUL, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "i * ( i ) - i ;",    OK, { ID, MUL, LEFT_BRACKET, ID, RIGHT_BRACKET, MINUS, ID, SEMICOLON } },
    { T_MATH, "i - i / ( i ) ;",    OK, { ID, MINUS, ID, DIV, LEFT_BRACKET, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "i ( i + i ) ;",  ER, { ID, LEFT_BRACKET, ID, PLUS, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( i * i ) i ;",  ER, { LEFT_BRACKET, ID, MUL, ID, RIGHT_BRACKET, ID, SEMICOLON } },
    { T_MATH, "i + ;",          ER, { ID, PLUS, SEMICOLON } },
    { T_MATH, "i - ;",          ER, { ID, MINUS, SEMICOLON } },
    { T_MATH, "i * ;",          ER, { ID, MUL, SEMICOLON } },
    { T_MATH, "i / ;",          ER, { ID, DIV, SEMICOLON } },
    { T_MATH, "+ i ;",          ER, { PLUS, ID, SEMICOLON } },
    { T_MATH, "- i ;",          ER, { MINUS, ID, SEMICOLON } },
    { T_MATH, "* i ;",          ER, { MUL, ID, SEMICOLON } },
    { T_MATH, "/ i ;",          ER, { DIV, ID, SEMICOLON } },
    { T_MATH, "+ ;",            ER, { PLUS, SEMICOLON } },
    { T_MATH, "- ;",            ER, { MINUS, SEMICOLON } },
    { T_MATH, "* ;",            ER, { MUL, SEMICOLON } },
    { T_MATH, "/ ;",            ER, { DIV, SEMICOLON } },
    { T_MATH, "str + str ;",    OK, { STRING_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "i + str ;",      OK, { ID, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "int + str ;",    OK, { INT_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "dbl + str ;",    OK, { DOUBLE_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "str - str ;",    OK, { STRING_LITERAL, MINUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "str * str ;",    OK, { STRING_LITERAL, MUL, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "str / str ;",    OK, { STRING_LITERAL, DIV, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "str + str + str ;",      OK, { STRING_LITERAL, PLUS, STRING_LITERAL, PLUS, STRING_LITERAL, SEMICOLON } },
    { T_MATH, "str + ( str ) ;",        OK, { STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "str + ( str + str ) ;",  OK, { STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, PLUS, STRING_LITERAL, RIGHT_BRACKET, SEMICOLON } },

    { T_BOOL, "i < i )",        OK, { ID, LESS, ID, RIGHT_BRACKET } },
    { T_BOOL, "i > i )",        OK, { ID, GREAT, ID, RIGHT_BRACKET } },
    { T_BOOL, "i <= i )",       OK, { ID, LESS_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, "i >= i )",       OK, { ID, GREAT_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, "i == i )",       OK, { ID, EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, "i != i )",       OK, { ID, N_EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, "i < )",          ER, { ID, LESS, RIGHT_BRACKET } },
    { T_BOOL, "i > )",          ER, { ID, GREAT, RIGHT_BRACKET } },
    { T_BOOL, "i <= )",         ER, { ID, LESS_EQ, RIGHT_BRACKET } },
    { T_BOOL, "i >= )",         ER, { ID, GREAT_EQ, RIGHT_BRACKET } },
    { T_BOOL, "i == )",         ER, { ID, EQUAL, RIGHT_BRACKET } },
    { T_BOOL, "i != )",         ER, { ID, N_EQUAL, RIGHT_BRACKET } },
    { T_BOOL, "< i )",          ER, { LESS, ID, RIGHT_BRACKET } },
    { T_BOOL, "> i )",          ER, { GREAT, ID, RIGHT_BRACKET } },
    { T_BOOL, "<= i )",         ER, { LESS_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, ">= i )",         ER, { GREAT_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, "== i )",         ER, { EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, "!= i )",         ER, { N_EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, "i < i < i )",    ER, { ID, LESS, ID, LESS, ID, RIGHT_BRACKET } },
    { T_BOOL, "i > i > i )",    ER, { ID, GREAT, ID, GREAT, ID, RIGHT_BRACKET } },
    { T_BOOL, "i <= i <= i )",  ER, { ID, LESS_EQ, ID, LESS_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, "i >= i >= i )",  ER, { ID, GREAT_EQ, ID, GREAT_EQ, ID, RIGHT_BRACKET } },
    { T_BOOL, "i == i == i )",  ER, { ID, EQUAL, ID, EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, "i != i != i )",  ER, { ID, N_EQUAL, ID, N_EQUAL, ID, RIGHT_BRACKET } },
    { T_BOOL, "str < str )",    OK, { STRING_LITERAL, LESS, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, "str > str )",    OK, { STRING_LITERAL, GREAT, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, "str <= str )",   OK, { STRING_LITERAL, LESS_EQ, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, "str >= str )",   OK, { STRING_LITERAL, GREAT_EQ, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, "str == str )",   OK, { STRING_LITERAL, EQUAL, STRING_LITERAL, RIGHT_BRACKET } },
    { T_BOOL, "str != str )",   OK, { STRING_LITERAL, N_EQUAL, STRING_LITERAL, RIGHT_BRACKET } },

    { T_BOOL, ")",              OK, { RIGHT_BRACKET } },
    { T_MATH, ";",              ER, { SEMICOLON } },
    { T_BOOL, "i )",            OK, { ID, RIGHT_BRACKET } },
    { T_BOOL, "i ) )",          OK, { ID, RIGHT_BRACKET, RIGHT_BRACKET } },
    { T_MATH, "i ;",            OK, { ID, SEMICOLON } },
    { T_MATH, "i ; ;",          OK, { ID, SEMICOLON, SEMICOLON } },
    { T_BOOL, "( i ) )",        OK, { LEFT_BRACKET, ID, RIGHT_BRACKET, RIGHT_BRACKET } },
    { T_MATH, "( i ) ;",        OK, { LEFT_BRACKET, ID, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "( i ;",          ER, { LEFT_BRACKET, ID, SEMICOLON } },

    { T_MATH, "i < int + ((i * (dbl - (i / i)))) ;", OK, { ID, LESS, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, ID, MINUS, LEFT_BRACKET, ID, DIV, ID, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
    { T_MATH, "i != int + ((dbl * (int - i) / int)) ;", OK, { ID, N_EQUAL, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, INT_LITERAL, MINUS, ID, RIGHT_BRACKET, DIV, INT_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET, SEMICOLON } },
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
    int i, result;

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

        if (tests[i].expr_type == T_BOOL)
            result = bool_expr();
        else if (tests[i].expr_type == T_MATH)
            result = math_expr();
        else
            result = -1;

        printf(COLOR_BLUE);
        printf("\n**************************************************************************\n");

        switch (result) {
            case SYNTAX_OK:
                printf(" parser result: SYNTAX_OK\n");
                break;
            case SYNTAX_ERROR:
                printf(" parser result: SYNTAX_ERROR\n");
                break;
            case SEMANTIC_ERROR:
                printf(" parser result: SEMANTIC_ERROR\n");
                break;
            default:
                printf(" parser result: other\n");
        }

        if (result - tests[i].expectedness == 0) {
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
