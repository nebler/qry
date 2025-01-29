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

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std {
template <> struct hash<Token> {
  size_t operator()(const Token &token) const {

    size_t h1 = std::hash<int>{}(static_cast<int>(token.getType()));
    size_t h2 = std::hash<std::string>{}(token.getText());
    return h1 ^ (h2 << 1);
  }
};
} // namespace std