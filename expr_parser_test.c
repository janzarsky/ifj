#include <stdio.h>
#include "expr_parser.h"
#include "parser_test.h"
#include "instrlist.h"

#define INPUT_SIZE 200

typedef struct {
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

#define NUM_TESTS 100

test_case_t tests[NUM_TESTS] = {
    { "<empty>",        ER, { END_OF_FILE } },
    { "int",            OK, { INT_LITERAL, END_OF_FILE } },
    { "double",         OK, { DOUBLE_LITERAL, END_OF_FILE } },
    { "string",         OK, { STRING_LITERAL, END_OF_FILE } },
    { "i",              OK, { ID, END_OF_FILE } },
    { "int int",        ER, { INT_LITERAL, INT_LITERAL, END_OF_FILE } },
    { "dbl dbl",        ER, { DOUBLE_LITERAL, DOUBLE_LITERAL, END_OF_FILE } },
    { "str str",        ER, { STRING_LITERAL, STRING_LITERAL, END_OF_FILE } },
    { "i i",            ER, { ID, ID, END_OF_FILE } },
    { "(",              ER, { LEFT_BRACKET, END_OF_FILE } },
    { ")",              ER, { RIGHT_BRACKET, END_OF_FILE } },
    { "()",             ER, { LEFT_BRACKET, RIGHT_BRACKET, END_OF_FILE } },
    { "( i",            ER, { LEFT_BRACKET, ID, END_OF_FILE } },
    { "i )",            ER, { ID, RIGHT_BRACKET, END_OF_FILE } },
    { "( i )",          OK, { LEFT_BRACKET, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "( int )",        OK, { LEFT_BRACKET, INT_LITERAL, RIGHT_BRACKET, END_OF_FILE } },
    { "( dbl )",        OK, { LEFT_BRACKET, DOUBLE_LITERAL, RIGHT_BRACKET, END_OF_FILE } },
    { "( str )",        OK, { LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, END_OF_FILE } },
    { "( ( i ) )",      OK, { LEFT_BRACKET, LEFT_BRACKET, ID, RIGHT_BRACKET, RIGHT_BRACKET, END_OF_FILE } },
    { "( ( str ) )",    OK, { LEFT_BRACKET, LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET, END_OF_FILE } },
    { "i + i",          OK, { ID, PLUS, ID, END_OF_FILE } },
    { "i - i",          OK, { ID, MINUS, ID, END_OF_FILE } },
    { "i * i",          OK, { ID, MUL, ID, END_OF_FILE } },
    { "i / i",          OK, { ID, DIV, ID, END_OF_FILE } },
    { "i + i + i",      OK, { ID, PLUS, ID, PLUS, ID, END_OF_FILE } },
    { "i - i - i",      OK, { ID, MINUS, ID, MINUS, ID, END_OF_FILE } },
    { "i * i * i",      OK, { ID, MUL, ID, MUL, ID, END_OF_FILE } },
    { "i / i / i",      OK, { ID, DIV, ID, DIV, ID, END_OF_FILE } },
    { "i + i * i",      OK, { ID, PLUS, ID, MUL, ID, END_OF_FILE } },
    { "i * i - i",      OK, { ID, MUL, ID, MINUS, ID, END_OF_FILE } },
    { "i - i / i",      OK, { ID, MINUS, ID, DIV, ID, END_OF_FILE } },
    { "i / i + i",      OK, { ID, DIV, ID, PLUS, ID, END_OF_FILE } },
    { "i + ( i + i )",  OK, { ID, PLUS, LEFT_BRACKET, ID, PLUS, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "i - ( i - i )",  OK, { ID, MINUS, LEFT_BRACKET, ID, MINUS, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "( i * i ) * i",  OK, { LEFT_BRACKET, ID, MUL, ID, RIGHT_BRACKET, MUL, ID, END_OF_FILE } },
    { "( i / i / i )",  OK, { LEFT_BRACKET, ID, DIV, ID, DIV, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "( i ) + i * i",  OK, { LEFT_BRACKET, ID, PLUS, ID, MUL, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "i * ( i ) - i",  OK, { ID, MUL, LEFT_BRACKET, ID, RIGHT_BRACKET, MINUS, ID, END_OF_FILE } },
    { "i - i / ( i )",  OK, { ID, MINUS, ID, DIV, LEFT_BRACKET, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "i ( i + i )",    ER, { ID, LEFT_BRACKET, ID, PLUS, ID, RIGHT_BRACKET, END_OF_FILE } },
    { "( i * i ) i",    ER, { LEFT_BRACKET, ID, MUL, ID, RIGHT_BRACKET, ID, END_OF_FILE } },
    { "i +",            ER, { ID, PLUS, END_OF_FILE } },
    { "i -",            ER, { ID, MINUS, END_OF_FILE } },
    { "i *",            ER, { ID, MUL, END_OF_FILE } },
    { "i /",            ER, { ID, DIV, END_OF_FILE } },
    { "+ i",            ER, { PLUS, ID, END_OF_FILE } },
    { "- i",            ER, { MINUS, ID, END_OF_FILE } },
    { "* i",            ER, { MUL, ID, END_OF_FILE } },
    { "/ i",            ER, { DIV, ID, END_OF_FILE } },
    { "+",              ER, { PLUS, END_OF_FILE } },
    { "-",              ER, { MINUS, END_OF_FILE } },
    { "*",              ER, { MUL, END_OF_FILE } },
    { "/",              ER, { DIV, END_OF_FILE } },
    { "str + str",      OK, { STRING_LITERAL, PLUS, STRING_LITERAL, END_OF_FILE } },
    { "i + str",        OK, { ID, PLUS, STRING_LITERAL, END_OF_FILE } },
    { "int + str",      OK, { INT_LITERAL, PLUS, STRING_LITERAL, END_OF_FILE } },
    { "dbl + str",      OK, { DOUBLE_LITERAL, PLUS, STRING_LITERAL, END_OF_FILE } },
    { "str - str",      OK, { STRING_LITERAL, MINUS, STRING_LITERAL, END_OF_FILE } },
    { "str * str",      OK, { STRING_LITERAL, MUL, STRING_LITERAL, END_OF_FILE } },
    { "str / str",      OK, { STRING_LITERAL, DIV, STRING_LITERAL, END_OF_FILE } },
    { "str + str + str",        OK, { STRING_LITERAL, PLUS, STRING_LITERAL, PLUS, STRING_LITERAL, END_OF_FILE } },
    { "str + ( str )",          OK, { STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, RIGHT_BRACKET, END_OF_FILE } },
    { "str + ( str + str )",    OK, { STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, PLUS, STRING_LITERAL, RIGHT_BRACKET, END_OF_FILE } },

    { "i < i",          OK, { ID, LESS, ID, END_OF_FILE } },
    { "i > i",          OK, { ID, GREAT, ID, END_OF_FILE } },
    { "i <= i",         OK, { ID, LESS_EQ, ID, END_OF_FILE } },
    { "i >= i",         OK, { ID, GREAT_EQ, ID, END_OF_FILE } },
    { "i == i",         OK, { ID, EQUAL, ID, END_OF_FILE } },
    { "i != i",         OK, { ID, N_EQUAL, ID, END_OF_FILE } },
    { "i <",            ER, { ID, LESS, END_OF_FILE } },
    { "i >",            ER, { ID, GREAT, END_OF_FILE } },
    { "i <=",           ER, { ID, LESS_EQ, END_OF_FILE } },
    { "i >=",           ER, { ID, GREAT_EQ, END_OF_FILE } },
    { "i ==",           ER, { ID, EQUAL, END_OF_FILE } },
    { "i !=",           ER, { ID, N_EQUAL, END_OF_FILE } },
    { "< i",            ER, { LESS, ID, END_OF_FILE } },
    { "> i",            ER, { GREAT, ID, END_OF_FILE } },
    { "<= i",           ER, { LESS_EQ, ID, END_OF_FILE } },
    { ">= i",           ER, { GREAT_EQ, ID, END_OF_FILE } },
    { "== i",           ER, { EQUAL, ID, END_OF_FILE } },
    { "!= i",           ER, { N_EQUAL, ID, END_OF_FILE } },
    { "i < i < i",      ER, { ID, LESS, ID, LESS, ID, END_OF_FILE } },
    { "i > i > i",      ER, { ID, GREAT, ID, GREAT, ID, END_OF_FILE } },
    { "i <= i <= i",    ER, { ID, LESS_EQ, ID, LESS_EQ, ID, END_OF_FILE } },
    { "i >= i >= i",    ER, { ID, GREAT_EQ, ID, GREAT_EQ, ID, END_OF_FILE } },
    { "i == i == i",    ER, { ID, EQUAL, ID, EQUAL, ID, END_OF_FILE } },
    { "i != i != i",    ER, { ID, N_EQUAL, ID, N_EQUAL, ID, END_OF_FILE } },
    { "str < str",      OK, { STRING_LITERAL, LESS, STRING_LITERAL, END_OF_FILE } },
    { "str > str",      OK, { STRING_LITERAL, GREAT, STRING_LITERAL, END_OF_FILE } },
    { "str <= str",     OK, { STRING_LITERAL, LESS_EQ, STRING_LITERAL, END_OF_FILE } },
    { "str >= str",     OK, { STRING_LITERAL, GREAT_EQ, STRING_LITERAL, END_OF_FILE } },
    { "str == str",     OK, { STRING_LITERAL, EQUAL, STRING_LITERAL, END_OF_FILE } },
    { "str != str",     OK, { STRING_LITERAL, N_EQUAL, STRING_LITERAL, END_OF_FILE } },

    { "i < int + ((i * (dbl - (i / i))))", OK, { ID, LESS, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, ID, MINUS, LEFT_BRACKET, ID, DIV, ID, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, END_OF_FILE } },
    { "i != int + ((dbl * (int - i) / int))", OK, { ID, N_EQUAL, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, INT_LITERAL, MINUS, ID, RIGHT_BRACKET, DIV, INT_LITERAL, RIGHT_BRACKET, RIGHT_BRACKET, END_OF_FILE } },
};

int test_tokens_counter = 0;
int test_number = 0;

int get_next_token() {
    int result = tests[test_number].tokens[test_tokens_counter];

    if (result != END_OF_FILE) {
        test_tokens_counter++;
    }

    return result;
}

int main(int __attribute__((unused)) argc, char __attribute__((unused)) **argv) {
    printf("**************************************************************************\n");
    printf(" Expression parser test\n");
    printf("**************************************************************************\n\n");

    int exit_code = 0;
    int i;

    for (i = 0; i < NUM_TESTS; i++) {
        if (tests[i].input[0] == 0) {
            break;
        }

        printf(COLOR_YELLOW);
        printf("**************************************************************************\n");
        printf(" Test n. %d\n", i + 1);
        printf(" input: '%s'\n", tests[i].input);
        printf("**************************************************************************\n\n");
        printf(COLOR_RESET);

        test_tokens_counter = 0;
        test_number = i;

        int result = math_expr();

        printf(COLOR_BLUE);
        printf("\n**************************************************************************\n");

        switch (result) {
            case SYNTAX_OK:
                printf(" parser result: SYNTAX_OK\n");
                break;
            case SYNTAX_ERROR:
                printf(" parser result: SYNTAX_ERROR\n");
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
