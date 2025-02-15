#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"

class FloatParselet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    return std::make_unique<FloatExpr>(std::stod(token.getText()));
  }
};