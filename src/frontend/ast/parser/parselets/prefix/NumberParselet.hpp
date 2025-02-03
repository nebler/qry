#pragma once

#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"

class NumberParselet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    // Convert the token's text to a number
    double value = std::stod(token.getText());
    return std::make_unique<NumberExpr>(value);
  }
};