#include <stdio.h>
#include "expr_parser.h"
#include "parser_test.h"
#include "instrlist.h"

//#define TEST_TOKENS_MAX 20
#define TEST_TOKENS_MAX 8

//char *input = "i < 123 + ((i * (42.5 - (i / i))))";
char *input = "\"asdf\" + (\"a\" + \"b\")";

int test_tokens[TEST_TOKENS_MAX] = {
    STRING_LITERAL, PLUS, LEFT_BRACKET, STRING_LITERAL, PLUS, STRING_LITERAL, RIGHT_BRACKET, END_OF_FILE
//    ID, LESS, INT_LITERAL, PLUS, LEFT_BRACKET, LEFT_BRACKET, DOUBLE_LITERAL, MUL, LEFT_BRACKET, ID, MINUS, LEFT_BRACKET, ID, DIV, ID, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, RIGHT_BRACKET, END_OF_FILE
};
int test_tokens_counter = 0;

int get_next_token() {
    int result = test_tokens[test_tokens_counter];

    if (test_tokens_counter < TEST_TOKENS_MAX - 1) {
        test_tokens_counter++;
    }

    return result;
}

int main(int __attribute__((unused)) argc, char __attribute__((unused)) **argv) {
    printf("**************************************************************************\n");
    printf(" Expression parser test\n");
    printf(" input: %s\n", input);
    printf("**************************************************************************\n");


    int result = math_expr();

    if (result == SYNTAX_OK)
        return 0;

    return -1;
}
