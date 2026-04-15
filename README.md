# Compiler Construction Project

This repository contains my Compiler Construction coursework implemented
in C++. The project is organized into multiple phases of compiler
development, including lexical analysis, syntax analysis, and
intermediate code generation for a custom teaching language.

## Overview

The project targets a custom language used in the course and includes:

-   **Lexical Analysis:** Table-driven lexical analyzer
-   **Syntax Analysis:** Recursive-descent parser with parse tree output
-   **Semantic Analysis / Code Generation:** Three-address code
    generation

The language includes custom keywords and constructs such as `Adadi`,
`Ashriya`, `Harf`, `Matn`, `Mantiqi`, `Agar`, `Wagarna`, `Marqazi`,
`input->`, and `output<-`.

------------------------------------------------------------------------

## Project Structure

    Lexical Analysis/
    ├── Lexer.cpp
    ├── FA_transition_table.xlsx
    ├── reserve_words.txt
    ├── test_code.txt
    ├── tokens.txt
    ├── symbol_table.txt
    ├── literal_table.txt
    ├── error.txt
    │
    Syntax Analysis/
    ├── Parser.cpp
    ├── reserveWords.txt
    ├── tokens.txt
    │
    Semantic Analysis/
    ├── compiler.cpp
    ├── sourcecode.txt
    ├── token.txt
    ├── threeAddrCode.txt
    │
    └── README.md

------------------------------------------------------------------------

## Project Phases

### Lexical Analysis

This phase implements a **table-driven lexical analyzer** using a finite
automaton approach in C++.

Features: - Tokenization of source code - Identification of: -
keywords - identifiers - numbers (integer, float, exponential) -
operators - punctuations - string literals - comments - Generates: -
`tokens.txt` - `symbol_table.txt` - `literal_table.txt` - `error.txt`

Also includes: - `FA_transition_table.xlsx` for transition design

------------------------------------------------------------------------

### Syntax Analysis

This phase implements a **recursive-descent parser**.

Features: - Parses token stream generated from lexical analysis -
Validates grammar structure - Supports: - functions - declarations -
argument lists - compound statements - `if-else`, `while`, `for` -
expressions - Outputs: - parse tree-style structure - syntax errors (if
any)

------------------------------------------------------------------------

### Semantic Analysis / Code Generation

This phase extends parsing with **intermediate code generation**.

Features: - Generates **three-address code (TAC)** - Supports: -
assignments - arithmetic expressions - relational expressions - control
flow (`if`, `while`) - Output: - `threeAddrCode.txt`

------------------------------------------------------------------------

## Authorship Note

This course project was completed across three phases:

-   **Lexical Analysis:** My individual work\
-   **Syntax Analysis:** My individual work\
-   **Semantic Analysis / Final Phase:** Group work

As allowed by the course, the final phase was built on top of my
teammate's earlier implementation. The final compiler stage was
developed collaboratively.

------------------------------------------------------------------------

## How to Run

### 1. Lexical Analysis

``` bash
cd "Lexical Analysis"
g++ Lexer.cpp -o lexer
./lexer
```

### 2. Syntax Analysis

``` bash
cd "Syntax Analysis"
g++ Parser.cpp -o parser
./parser
```

### 3. Semantic Analysis / Code Generation

``` bash
cd "Semantic Analysis"
g++ compiler.cpp -o compiler
./compiler
```

-----------------------------------------------------------------------
