// Lexer.cpp
#include "Lexer.hpp"
#include <cctype>
#include <iostream>

int Lexer::advance() { return input->get(); }

int Lexer::gettok() {
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

    if (identifierStr == "def") {
      return tok_def;
    }
    if (identifierStr == "extern") {
      return tok_extern;
    }

    std::cout << "Identifier:" << tok_identifier << std::endl;
    return tok_identifier;
  }

  // Handle numbers [0-9.]+
  if (isdigit(lastChar) || lastChar == '.') {
    std::string numStr;
    do {
      numStr += lastChar;
      lastChar = advance();
    } while (isdigit(lastChar) || lastChar == '.');

    numVal = strtod(numStr.c_str(), nullptr);
    std::cout << "Number:" << numVal << std::endl;

    return tok_number;
  }

  // Check for end of file
  if (lastChar == EOF) {
    std::cout << "End of file" << std::endl;
    return tok_eof;
  }

  // Otherwise, return the character as its ASCII value
  int thisChar = lastChar;
  lastChar = advance();
  return thisChar;
}
