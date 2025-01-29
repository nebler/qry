#pragma once

#include "frontend/ast/lexer/token/TokenType.hpp"
#include <string>
class Token {
  TokenType tokenType;

  std::string text;

public:
  Token(TokenType tokenType, std::string text)
      : tokenType(tokenType), text(text) {};

  Token() : tokenType(tok_undefined), text("") {}

  TokenType getType() const { return tokenType; }

  std::string getText() const { return text; }

  bool operator==(const Token &other) const {
    return tokenType == other.tokenType && text == other.text;
  }

  bool operator!=(const Token &other) const { return !(*this == other); }
};
