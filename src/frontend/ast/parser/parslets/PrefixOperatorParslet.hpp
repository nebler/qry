#pragma once

#include "PrefixParselet.hpp"

/**
 * Generic prefix parselet for an unary arithmetic operator. Parses prefix
 * unary "-", "+", "~", and "!" expressions.
 */
class PrefixOperatorParselet : public PrefixParselet {
public:
  PrefixOperatorParselet(int precedence) : mPrecedence(precedence) {}

  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      Token token) const override {
    std::unique_ptr<Expr> right = parser.parseExpression(mPrecedence);

    return std::unique_ptr<Expr>(new PrefixExpr(token, std::move(right)));
  }

private:
  const int mPrecedence;
};
