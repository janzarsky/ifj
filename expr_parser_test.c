#include <stdio.h>
#include "expr_parser.h"
#include "parser_test.h"
#include "instrlist.h"

#define TEST_TOKENS_MAX 20
char *input = "i < i + ((i * (i - (i / i))))";
int test_tokens[TEST_TOKENS_MAX] = {
    ID, LESS, ID, PLUS, LEFT_BRACKET, LEFT_BRACKET, ID, MUL, LEFT_BRACKET, ID,
    MINUS, LEFT_BRACKET, ID, DIV, ID, RIGHT_BRACKET, RIGHT_BRACKET,
    RIGHT_BRACKET, RIGHT_BRACKET, END_OF_FILE
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
