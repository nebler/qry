#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"

class StringParselet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    // Parse Text until another " appears thats the value of the string

    return std::make_unique<StringExpr>(token.getText());
  }
};