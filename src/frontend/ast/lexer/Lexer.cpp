// Lexer.cpp
#include "Lexer.hpp"
#include "frontend/ast/lexer/Token.hpp"
#include <cctype>
#include <iostream>

int Lexer::advance() { return input->get(); }

/*
    simple peek method
    returns the next character without eating it.
*/
int Lexer::peek() {
  // Save current position
  // tellg gives me the current position
  int pos = input->tellg();
  // Get next character
  int nextChar = input->get();
  // Restore position
  // seekg puts the position back to where it was before
  input->seekg(pos);
  return nextChar;
}

Token Lexer::lookAhead(int distance) {
  // Save current position
  // tellg gives me the current position
  int pos = input->tellg();
  // Get next character
  Token token;
  for (int i = 0; i < distance; i++) {
    token = gettok();
  }
  // Restore position
  // seekg puts the position back to where it was before
  input->seekg(pos);
  return token;
}

Token Lexer::gettok() {
  // Skip whitespace
  while (isspace(lastChar)) {
    lastChar = advance();
  }
  // Handle identifiers [a-zA-Z][a-zA-Z0-9]*
  if (isalpha(lastChar)) {
    identifierStr = lastChar;
    while (isalnum((lastChar = advance()))) {
      identifierStr += lastChar;
    }

    if (identifierStr == "def" && !isalnum(peek())) {
      return tok_def;
    }
    if (identifierStr == "extern" && !isalnum(peek())) {
      return tok_extern;
    }
    if (identifierStr == "int" && !isalnum(peek())) {
      return tok_int;
    }
    if (identifierStr == "bool" && !isalnum(peek())) {
      return tok_bool;
    }
    if (identifierStr == "float" && !isalnum(peek())) {
      return tok_float;
    }

    return tok_identifier;
  }

  // Handle numbers [0-9.]+
  if (isdigit(lastChar) || lastChar == '.') {
    std::string numStr;
    do {
      numStr += lastChar;
      lastChar = advance();
    } while (isdigit(lastChar) || lastChar == '.');

    setNumVal(strtod(numStr.c_str(), nullptr));
    return tok_number;
  }

  if (identifierStr == "+") {
    return tok_plus;
  }

  if (identifierStr == "-") {
    return tok_minus;
  }

  if (identifierStr == ",") {
    return tok_comma;
  }

  if (identifierStr == "*") {
    return tok_asterix;
  }

  if (identifierStr == "^") {
    return tok_caret;
  }

  if (identifierStr == "~") {
    return tok_caret;
  }

  if (identifierStr == "!") {
    return tok_bang;
  }

  if (identifierStr == "/") {
    return tok_slash;
  }

  if (identifierStr == ")") {
    return tok_right_paren;
  }

  if (identifierStr == "()") {
    return tok_left_paren;
  }
  // Check for end of file
  if (lastChar == EOF) {
    std::cout << "End of file" << std::endl;
    return tok_eof;
  }

  // Otherwise, return the character as its ASCII value
  // int thisChar = lastChar;
  // lastChar = advance();
  // Maybe we should return something else here? Maybe tokens should actually
  // hold some type of value?
  return tok_undefined;
}
