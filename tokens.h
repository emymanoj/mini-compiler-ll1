#ifndef TOKENS_H
#define TOKENS_H

// ─── Token Types ───────────────────────────────────────────────
typedef enum {
    TOKEN_NUM,      // [0-9]+
    TOKEN_ID,       // [a-zA-Z][a-zA-Z0-9]*
    TOKEN_OP,       // + - * /
    TOKEN_ASSIGN,   // =
    TOKEN_LPAREN,   // (
    TOKEN_RPAREN,   // )
    TOKEN_EOF,      // End of input
    TOKEN_UNKNOWN   // Unrecognized character
} TokenType;

// ─── Token Structure ───────────────────────────────────────────
typedef struct {
    TokenType type;
    char value[64];  // Stores the actual matched text
} Token;

// ─── Token Name (for printing) ─────────────────────────────────
static const char* token_names[] = {
    "NUM", "ID", "OP", "ASSIGN", "LPAREN", "RPAREN", "EOF", "UNKNOWN"
};

#endif