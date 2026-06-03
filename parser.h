#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Grammar after removing left recursion:
 *
 *   S  → ID = E
 *   E  → T E'
 *   E' → + T E' | - T E' | ε
 *   T  → F T'
 *   T' → * F T' | / F T' | ε
 *   F  → ( E ) | NUM | ID
 */

// ─── Parser State ──────────────────────────────────────────────
static Token current_token;
static int   step_count = 0;

// ─── Helpers ───────────────────────────────────────────────────
void parser_error(const char *msg) {
    printf("[ERROR] %s (got '%s')\n", msg, current_token.value);
    exit(1);
}

void match(TokenType expected) {
    if (current_token.type == expected) {
        printf("  Step %2d | Match  %-8s '%s'\n",
               ++step_count,
               token_names[expected],
               current_token.value);
        current_token = next_token();
    } else {
        printf("[ERROR] Expected %s but got %s ('%s')\n",
               token_names[expected],
               token_names[current_token.type],
               current_token.value);
        exit(1);
    }
}

// ─── Forward declarations ──────────────────────────────────────
void parse_S();
void parse_E();
void parse_E_prime();
void parse_T();
void parse_T_prime();
void parse_F();

// ─── S → ID = E ───────────────────────────────────────────────
void parse_S() {
    printf("  Step %2d | Apply  S  → ID = E\n", ++step_count);
    match(TOKEN_ID);
    match(TOKEN_ASSIGN);
    parse_E();
}

// ─── E → T E' ─────────────────────────────────────────────────
void parse_E() {
    printf("  Step %2d | Apply  E  → T E'\n", ++step_count);
    parse_T();
    parse_E_prime();
}

// ─── E' → + T E' | - T E' | ε ─────────────────────────────────
void parse_E_prime() {
    if (current_token.type == TOKEN_OP &&
       (current_token.value[0] == '+' || current_token.value[0] == '-')) {
        printf("  Step %2d | Apply  E' → %s T E'\n",
               ++step_count, current_token.value);
        match(TOKEN_OP);
        parse_T();
        parse_E_prime();
    } else {
        // ε production
        printf("  Step %2d | Apply  E' → ε\n", ++step_count);
    }
}

// ─── T → F T' ─────────────────────────────────────────────────
void parse_T() {
    printf("  Step %2d | Apply  T  → F T'\n", ++step_count);
    parse_F();
    parse_T_prime();
}

// ─── T' → * F T' | / F T' | ε ─────────────────────────────────
void parse_T_prime() {
    if (current_token.type == TOKEN_OP &&
       (current_token.value[0] == '*' || current_token.value[0] == '/')) {
        printf("  Step %2d | Apply  T' → %s F T'\n",
               ++step_count, current_token.value);
        match(TOKEN_OP);
        parse_F();
        parse_T_prime();
    } else {
        // ε production
        printf("  Step %2d | Apply  T' → ε\n", ++step_count);
    }
}

// ─── F → ( E ) | NUM | ID ─────────────────────────────────────
void parse_F() {
    if (current_token.type == TOKEN_LPAREN) {
        printf("  Step %2d | Apply  F  → ( E )\n", ++step_count);
        match(TOKEN_LPAREN);
        parse_E();
        match(TOKEN_RPAREN);
    } else if (current_token.type == TOKEN_NUM) {
        printf("  Step %2d | Apply  F  → NUM\n", ++step_count);
        match(TOKEN_NUM);
    } else if (current_token.type == TOKEN_ID) {
        printf("  Step %2d | Apply  F  → ID\n", ++step_count);
        match(TOKEN_ID);
    } else {
        parser_error("Expected (, NUM, or ID in F");
    }
}

// ─── Public Entry Point ────────────────────────────────────────
void parse(const char *input) {
    printf("\n=== PARSE STEPS ===\n");
    step_count = 0;
    lexer_init(input);
    current_token = next_token();
    parse_S();
    if (current_token.type != TOKEN_EOF) {
        parser_error("Unexpected tokens after valid expression");
    }
    printf("  ✓ Parse successful!\n");
}

// ─── Print FIRST and FOLLOW Sets ──────────────────────────────
void print_first_follow() {
    printf("\n=== FIRST and FOLLOW Sets ===\n");
    printf("┌──────────┬──────────────────────────┬──────────────────────────┐\n");
    printf("│ NonTerm  │ FIRST                    │ FOLLOW                   │\n");
    printf("├──────────┼──────────────────────────┼──────────────────────────┤\n");
    printf("│ S        │ { ID }                   │ { $ }                    │\n");
    printf("│ E        │ { (, NUM, ID }           │ { ), $ }                 │\n");
    printf("│ E'       │ { +, -, ε }              │ { ), $ }                 │\n");
    printf("│ T        │ { (, NUM, ID }           │ { +, -, ), $ }           │\n");
    printf("│ T'       │ { *, /, ε }              │ { +, -, ), $ }           │\n");
    printf("│ F        │ { (, NUM, ID }           │ { *, /, +, -, ), $ }     │\n");
    printf("└──────────┴──────────────────────────┴──────────────────────────┘\n");

    printf("\n=== LL(1) PARSING TABLE ===\n");
    printf("┌──────┬────────────┬────────────┬────────────┬────────┬────────┬────────┬───────┐\n");
    printf("│      │  ID        │  NUM       │  (         │  +     │  -     │  *  /  │  )  $ │\n");
    printf("├──────┼────────────┼────────────┼────────────┼────────┼────────┼────────┼───────┤\n");
    printf("│ S    │ S→ID=E     │            │            │        │        │        │       │\n");
    printf("│ E    │ E→TE'      │ E→TE'      │ E→TE'      │        │        │        │       │\n");
    printf("│ E'   │            │            │            │ E'→+TE'│ E'→-TE'│        │ E'→ε  │\n");
    printf("│ T    │ T→FT'      │ T→FT'      │ T→FT'      │        │        │        │       │\n");
    printf("│ T'   │            │            │            │ T'→ε   │ T'→ε   │T'→*FT' │ T'→ε  │\n");
    printf("│ F    │ F→ID       │ F→NUM      │ F→(E)      │        │        │        │       │\n");
    printf("└──────┴────────────┴────────────┴────────────┴────────┴────────┴────────┴───────┘\n");
}

#endif