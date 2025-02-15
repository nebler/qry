#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"

class IntParselet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    return std::make_unique<IntExpr>(std::stoi(token.getText()));
  }
};