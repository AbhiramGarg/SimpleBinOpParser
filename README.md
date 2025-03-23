# Simple Binary Expression Parser in C++

This repository contains a simple binary expression parser implemented in C++. It demonstrates the basic principles of lexical analysis (tokenization) and parsing, transforming a string input representing a mathematical expression into an Abstract Syntax Tree (AST).

## Features

* **Lexical Analysis (Tokenization):**
    * The lexer identifies and classifies tokens such as numbers, identifiers, and operators.
    * It handles whitespace and basic error checking.
* **Parsing:**
    * The parser constructs an AST representing the structure of the input expression.
    * It supports binary operators with precedence (e.g., `+`, `-`, `*`, `/`, `<`).
    * Handles parenthesis to control operator precedence.
    * Supports numbers and variable identifiers.
* **Abstract Syntax Tree (AST):**
    * The AST is represented using a hierarchy of C++ classes (`NumberExprAST`, `VariableExprAST`, `BinaryExprAST`).
    * The code includes a print function to display the generated AST.
* **Error Handling:**
    * Basic error reporting for invalid tokens and syntax errors.

## Code Structure

* **Lexer:**
    * `gettok()`: Reads the input buffer and returns the next token.
    * Handles number, identifier and operator identification.
* **Parser:**
    * `ParseNumberExpr()`, `ParseIdentifierExpr()`, `ParseParenExpr()`, `ParsePrimary()`, `parseExpression()`, `ParseBinOpRHS()`: Functions responsible for parsing different parts of the expression and constructing the AST.
    * `BinopPrecedence`: A map to define operator precedence.
* **AST:**
    * `ExprAST`: Base class for all expression nodes.
    * `NumberExprAST`, `VariableExprAST`, `BinaryExprAST`: Classes representing specific expression types.
* **Main Loop:**
    * Reads input expressions from standard input.
    * Calls the lexer and parser to generate the AST.
    * Prints the generated AST or error messages.

## Usage

1.  Clone the repository.
2.  Compile the C++ code using a C++ compiler (e.g., g++):
    ```bash
    g++ main.cpp -o parser
    ```
3.  Run the executable:
    ```bash
    ./parser
    ```
