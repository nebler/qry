#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"

class BoolParselet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    return std::make_unique<BoolExpr>(token.getText() == "true");
  }
};