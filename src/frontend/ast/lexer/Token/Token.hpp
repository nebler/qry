#pragma once

#include "frontend/ast/lexer/Token/TokenType.hpp"
#include <string>
class Token {
  TokenType tokenType;

  std::string text;

public:
  Token(TokenType tokenType, std::string text)
      : tokenType(tokenType), text(text) {};

  // Getters
  TokenType getType() const { return tokenType; }

  std::string getText() const { return text; }

  // Equality operator
  bool operator==(const Token &other) const {
    return tokenType == other.tokenType && text == other.text;
  }

  // Optional: Inequality operator
  bool operator!=(const Token &other) const { return !(*this == other); }
};
