#pragma once

/**
 * Generic prefix parselet for an unary arithmetic operator. Parses prefix
 * unary "-", "+", "~", and "!" expressions.
 */
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
class PrefixOperatorParselet : public PrefixParselet {
public:
  PrefixOperatorParselet(int precedence) : mPrecedence(precedence) {}

  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      Token token) const override {
    // To handle right-associative operators like "^", we allow a slightly
    // lower precedence when parsing the right-hand side. This will let a
    // parselet with the same precedence appear on the right, which will then
    // take *this* parselet's result as its left-hand argument.
    std::unique_ptr<Expr> right = parser.parseExpression(mPrecedence);

    return std::unique_ptr<PrefixExpr>(new PrefixExpr(token, std::move(right)));
  }

private:
  const int mPrecedence;
};
