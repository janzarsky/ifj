#include <stdio.h>
#include "expr_parser.h"
#include "parser_test.h"

enum token_t { T_PLUS, T_MUL, T_LEFT_BRACKET, T_RIGHT_BRACKET, T_ID, T_END, T_MAX };
#define TEST_TOKENS_MAX 6
int test_tokens[TEST_TOKENS_MAX] = {
    T_ID, T_PLUS, T_ID, T_MUL, T_ID, T_END
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
    int result = math_expr();

    if (result == SYNTAX_OK)
        return 0;

    return -1;
}
