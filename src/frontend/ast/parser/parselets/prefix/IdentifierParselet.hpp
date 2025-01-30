#pragma once

/**
 * Simple parselet for a named variable like "abc".
 */
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
class IdentifierParselet : public PrefixParselet {
  virtual std::unique_ptr<Expr> parse(Parser & /*parser*/,
                                      Token token) const override {
    return std::unique_ptr<IdentifierExpr>(new IdentifierExpr(token.getText()));
  }
};
