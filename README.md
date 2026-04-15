# Compiler Construction Project

This repository contains my Compiler Construction coursework implemented in C++. The project is organized around multiple phases of compiler development, including lexical analysis, syntax analysis, and intermediate code generation for a custom teaching language.

## Overview

The project targets a custom language used in the course and includes:

- **Phase 1:** Table-driven lexical analyzer
- **Phase 2:** Recursive-descent parser with parse tree style output
- **Phase 3:** Translation/code-generation phase producing three-address code

The language includes custom keywords and constructs such as `Adadi`, `Ashriya`, `Harf`, `Matn`, `Mantiqi`, `Agar`, `Wagarna`, `Marqazi`, `input->`, and `output<-`.

## Project Phases

### Phase 1 - Lexical Analysis

The first phase implements a scanner/lexical analyzer using the **dynamic table-driven method** in C++. The assignment required:

- regular definitions for lexical categories
- a transition diagram / finite automaton
- implementation of the FA using a table-driven approach
- generation of output files for tokens, symbol table, literal table, and lexical errors

The lexer handles:

- identifiers
- numbers, including signed, floating-point, and exponential forms
- punctuations
- operators
- keywords
- string literals
- comments
- lexical errors

### Phase 2 - Parsing

The second phase extends the compiler front-end with syntax analysis. The parser is implemented in C++ and follows a recursive-descent style. It reads tokenized input and validates whether the source conforms to the grammar. It also prints a structured parse tree style output for grammar productions.

Supported constructs in the uploaded parser include:

- function structure
- argument lists
- declarations
- identifier lists
- compound statements
- `for`, `while`, and `if-else`
- expressions and optional expressions

### Phase 3 - Intermediate Code Generation

The third phase builds on parsing and introduces code generation using a translation scheme. The uploaded grammar for Phase 3 includes:

- statements
- compound statements
- `while`
- `if-else`
- assignment expressions
- relational expressions
- arithmetic expressions
- factors over identifiers, numbers, and parenthesized expressions

The output of this phase is **three-address code (TAC)**.

## Authorship Note

This course project was completed across three phases.

- **Phase 1:** My individual work
- **Phase 2:** My individual work
- **Phase 3:** Group work completed with my teammate

As permitted by the course policy for the final phase, the group selected one earlier codebase as the foundation for Phase 3. For the final phase, the implementation was built on top of my teammate's earlier base, and then extended for the required translation/code-generation work.

This repository is meant to document the full coursework context clearly and honestly.

## Files in This Repository

This repository currently keeps files in the **root directory** so the existing code works without path changes.

Typical files include:

- `Lexer.cpp`
- `Parser.cpp`
- `compiler.cpp`
- `CC Phase 1 (1).pdf`
- `Grammar for Project Phase 3 VF.pdf`
- supporting input/output text files used by the programs

Depending on the phase, the code reads and/or writes files such as:

### Lexer files
- `test_code_22l6681.txt`
- `reserve_words_22l6681.txt`
- `tokens_22l6681.txt`
- `symbol_table_22l6681.txt`
- `literal_table_22l6681.txt`
- `error_22l6681.txt`

### Parser / Compiler files
- `token.txt`
- `sourcecode.txt`
- `literal.txt`
- `symbolTable.txt`
- `error.txt`
- `code.tac`

## Supported Language Features

Across the uploaded files, the compiler project supports a custom language with features such as:

- primitive/custom types like `Adadi`, `Ashriya`, `Harf`, `Matn`, `Mantiqi`
- identifiers and numeric literals
- arithmetic operators
- relational operators
- logical operators
- punctuation symbols and delimiters
- assignment
- function structure
- declarations
- control flow:
  - `Agar` / `Wagarna`
  - `while`
  - `for`
- compound statements using braces
- translation to three-address code in the final phase

## How to Run

> Note: file names are currently hardcoded in the source files, so keep the required input files in the same directory as the `.cpp` file and executable.

### Compile Phase 1 Lexer
```bash
g++ Lexer.cpp -o lexer
./lexer
