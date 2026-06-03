#  Mini Compiler LL(1)

A lightweight compiler front-end written in C that performs **lexical analysis** and **LL(1) predictive parsing** on simple assignment expressions. Built as a learning-oriented project to demonstrate the core phases of compilation theory.

---

## Overview

The Mini Compiler takes a single-line arithmetic assignment expression as input and walks it through two classic compiler phases:

1. **Lexical Analysis** — tokenizes the input into a labeled token stream
2. **Syntax Analysis** — validates the token stream against an LL(1) grammar and prints each parse step

It also displays the computed **FIRST/FOLLOW sets** and the **LL(1) parsing table** for the grammar used.

---

## Project Structure

```
lpproject/
├── main.c          # Entry point — drives lexing, parsing, and output
├── tokens.h        # Token types, Token struct, and token name table
├── lexer.h         # Lexer implementation (tokenizer + token stream printer)
├── parser.h        # LL(1) recursive-descent parser + FIRST/FOLLOW display
└── compiler.exe    # Pre-built Windows executable
```

---

## Grammar

The parser implements the following **left-recursion-free** grammar:

```
S  → ID = E
E  → T E'
E' → + T E' | - T E' | ε
T  → F T'
T' → * F T' | / F T' | ε
F  → ( E ) | NUM | ID
```

### Supported Token Types

| Token     | Pattern                          |
|-----------|----------------------------------|
| `NUM`     | One or more digits: `[0-9]+`     |
| `ID`      | Letter followed by alphanumerics: `[a-zA-Z][a-zA-Z0-9]*` |
| `OP`      | Arithmetic operator: `+ - * /`   |
| `ASSIGN`  | Assignment: `=`                  |
| `LPAREN`  | Left parenthesis: `(`            |
| `RPAREN`  | Right parenthesis: `)`           |
| `EOF`     | End of input                     |
| `UNKNOWN` | Any unrecognized character       |

---

## Usage

### Run the Pre-built Executable (Windows)

```cmd
compiler.exe
```

### Compile from Source (GCC)

```bash
gcc main.c -o compiler
./compiler
```

> All logic lives in header files, so only `main.c` needs to be passed to the compiler.

### Example Session

```
╔══════════════════════════════════════════╗
║   Mini Compiler                          ║
╚══════════════════════════════════════════╝

Enter expression: x = 3 + (a * 2)

Input: "x = 3 + (a * 2)"

=== TOKEN STREAM ===
<ID, x>
<ASSIGN, =>
<NUM, 3>
<OP, +>
<LPAREN, (>
<ID, a>
<OP, *>
<NUM, 2>
<RPAREN, )>
<EOF, EOF>

=== FIRST and FOLLOW Sets ===
...

=== LL(1) PARSING TABLE ===
...

=== PARSE STEPS ===
  Step  1 | Apply  S  → ID = E
  Step  2 | Match  ID       'x'
  ...
  ✓ Parse successful!
```

---

## Compiler Phases Demonstrated

### Phase 1 — Lexical Analysis (`lexer.h`)
- Scans the input character by character
- Recognizes integers, identifiers, operators, parentheses, and assignment
- Skips whitespace
- Emits a `<TokenType, value>` stream

### Phase 2 — Syntax Analysis (`parser.h`)
- Implements a **recursive-descent LL(1) parser**
- Prints each grammar rule application and token match step-by-step
- Displays pre-computed **FIRST** and **FOLLOW** sets
- Displays the **LL(1) parsing table**
- Reports descriptive errors on invalid input

---

## Requirements

| Requirement | Details |
|-------------|---------|
| Compiler    | GCC (any modern version) or compatible C99 compiler |
| OS          | Linux, macOS, or Windows |
| Dependencies| None (standard C library only) |

---

## Concepts Covered

- Finite automaton-based lexing
- Context-free grammars (CFG)
- Left-recursion elimination
- FIRST and FOLLOW set computation
- LL(1) parse table construction
- Recursive-descent parsing

---

## License

This project is intended for educational use. Feel free to use and modify it for learning purposes.
