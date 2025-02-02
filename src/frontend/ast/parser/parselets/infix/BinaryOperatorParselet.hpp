#pragma once

#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include <memory>
class BinaryOperatorParselet : public InfixParselet {
public:
  BinaryOperatorParselet(int precedence, bool isRight)
      : mPrecedence(precedence), mIsRight(isRight) {}

  std::unique_ptr<Expr> parse(Parser &parser, std::unique_ptr<Expr> left,
                              Token token) const override {
    // Handle right-associative operators by adjusting precedence
    std::unique_ptr<Expr> right =
        parser.parseExpression(mPrecedence - (mIsRight ? 1 : 0));

    // Create the appropriate binary expression based on the token type
    switch (token.getType()) {
    case tok_plus:
      return std::make_unique<PlusBinaryExpr>(std::move(left),
                                              std::move(right));

    case tok_minus:
      return std::make_unique<MinusBinaryExpr>(std::move(left),
                                               std::move(right));

    case tok_slash:
      return std::make_unique<DivisionBinaryExpr>(std::move(left),
                                                  std::move(right));

    case tok_caret:
      return std::make_unique<ExponentBinaryExpr>(std::move(left),
                                                  std::move(right));

    case tok_asterix:
      return std::make_unique<MultiplicationBinaryExpr>(std::move(left),
                                                        std::move(right));

    default:
      throw std::runtime_error("Unexpected binary operator: " +
                               token.getText());
    }
  }

  int getPrecedence() const override { return mPrecedence; }

private:
  const int mPrecedence;
  const bool mIsRight;
};