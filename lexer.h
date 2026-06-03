#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// ─── Lexer State ───────────────────────────────────────────────
static const char *input_ptr;  // Points to current position in input

// Initialize the lexer with an input string
void lexer_init(const char *input) {
    input_ptr = input;
}

// ─── Core Lexer Function ───────────────────────────────────────
// Reads the next token from input
Token next_token() {
    Token tok;
    tok.value[0] = '\0';

    // Skip whitespace
    while (*input_ptr == ' ' || *input_ptr == '\t' || *input_ptr == '\n') {
        input_ptr++;
    }

    // End of input
    if (*input_ptr == '\0') {
        tok.type = TOKEN_EOF;
        strcpy(tok.value, "EOF");
        return tok;
    }

    // ── Rule: NUM → [0-9]+
    if (isdigit(*input_ptr)) {
        int i = 0;
        while (isdigit(*input_ptr)) {
            tok.value[i++] = *input_ptr++;
        }
        tok.value[i] = '\0';
        tok.type = TOKEN_NUM;
        return tok;
    }

    // ── Rule: ID → [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(*input_ptr)) {
        int i = 0;
        while (isalnum(*input_ptr)) {
            tok.value[i++] = *input_ptr++;
        }
        tok.value[i] = '\0';
        tok.type = TOKEN_ID;
        return tok;
    }

    // ── Rule: ASSIGN → =
    if (*input_ptr == '=') {
        tok.type = TOKEN_ASSIGN;
        tok.value[0] = *input_ptr++;
        tok.value[1] = '\0';
        return tok;
    }

    // ── Rule: OP → [+\-*/]
    if (*input_ptr == '+' || *input_ptr == '-' ||
        *input_ptr == '*' || *input_ptr == '/') {
        tok.type = TOKEN_OP;
        tok.value[0] = *input_ptr++;
        tok.value[1] = '\0';
        return tok;
    }

    // ── Rule: LPAREN → (
    if (*input_ptr == '(') {
        tok.type = TOKEN_LPAREN;
        tok.value[0] = *input_ptr++;
        tok.value[1] = '\0';
        return tok;
    }

    // ── Rule: RPAREN → )
    if (*input_ptr == ')') {
        tok.type = TOKEN_RPAREN;
        tok.value[0] = *input_ptr++;
        tok.value[1] = '\0';
        return tok;
    }

    // Unknown character
    tok.type = TOKEN_UNKNOWN;
    tok.value[0] = *input_ptr++;
    tok.value[1] = '\0';
    return tok;
}

// ─── Print Token Stream ────────────────────────────────────────
void print_token_stream(const char *input) {
    printf("\n=== TOKEN STREAM ===\n");
    lexer_init(input);
    Token tok;
    do {
        tok = next_token();
        printf("<%s, %s>\n", token_names[tok.type], tok.value);
    } while (tok.type != TOKEN_EOF);
}

#endif