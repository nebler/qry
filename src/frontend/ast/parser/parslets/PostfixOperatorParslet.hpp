
#pragma once

#include "../Parser.hpp"
#include "InfixParslet.hpp"

class PostfixOperatorParselet : public InfixParselet {
public:
  PostfixOperatorParselet(int precedence) : mPrecedence(precedence) {}
  std::unique_ptr<Expr> parse(Parser &parser, std::unique_ptr<Expr> left,
                              Token token) override {
    return std::unique_ptr<PostfixExpr>(
        new PostfixExpr(std::move(left), token));
  };

  virtual int getPrecedence() const override { return mPrecedence; }

private:
  const int mPrecedence;
};
