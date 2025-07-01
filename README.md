# CS 236 Project: Scanner, Parser, and Interpreter

## Overview

This project implements a lexical scanner, parser, and interpreter as part of BYU’s CS 236 course (Theory of Computation). The goal is to process a simplified domain-specific language by:

- **Scanning** input text into tokens
- **Parsing** tokens into a structured representation based on grammar rules
- **Interpreting** the parsed structure to perform queries or output results

---

## Components

### Scanner

- Converts input text into tokens.
- Recognizes keywords (`Schemes`, `Facts`, `Rules`, `Queries`), identifiers, symbols, strings, comments, and undefined tokens.
- Handles whitespace and tracks line numbers.
- Implements tokenization rules without regular expressions using an if-else ladder.

### Parser

- Reads the token stream produced by the scanner.
- Validates and builds a parse tree or equivalent data structure based on the grammar rules defined in the project.
- Detects syntax errors and provides meaningful error messages.
- Follows context-free grammar rules to ensure the input is structurally correct.

### Interpreter

- Processes the parse tree or data structures produced by the parser.
- Executes queries or commands defined by the language.
- Outputs results such as relational data or error messages.
- Implements any semantic checks or logic required by the language specification.
