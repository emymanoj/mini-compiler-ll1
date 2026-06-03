#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main() {
    char input[256];

    printf("╔══════════════════════════════════════════╗\n");
    printf("║   Mini Compiler                          ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    printf("\nEnter expression: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character from fgets
    int len = 0;
    while (input[len] != '\0') len++;
    if (len > 0 && input[len - 1] == '\n')
        input[len - 1] = '\0';

    printf("\nInput: \"%s\"\n", input);

    // Step 1: Lexical Analysis - print token stream
    print_token_stream(input);

    // Step 2: Print FIRST/FOLLOW and LL(1) Table
    print_first_follow();

    // Step 3: Syntax Analysis - parse and show steps
    parse(input);

    return 0;
}