#pragma once

#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"

class StringParselet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    // Parse Text until another " appears thats the value of the string
    token = parser.consume();
    std::string value;
    while (token.getType() != tok_dobule_quotes) {
      value += token.getText();
      token = parser.consumeText();
    }
    return std::make_unique<StringExpr>(value);
  }
};