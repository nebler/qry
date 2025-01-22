#include "frontend/ast/parser/parslets/InfixParslet.hpp"
class BinaryOperatorParselet : public InfixParselet {
public:
  BinaryOperatorParselet(int precedence, bool isRight)
      : mPrecedence(precedence), mIsRight(isRight) {}

  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      std::unique_ptr<Expr> left,
                                      Token token) const override {
    // To handle right-associative operators like "^", we allow a slightly
    // lower precedence when parsing the right-hand side. This will let a
    // parselet with the same precedence appear on the right, which will then
    // take *this* parselet's result as its left-hand argument.
    std::unique_ptr<Expr> right =
        parser.parseExpression(mPrecedence - (mIsRight ? 1 : 0));

    return std::unique_ptr<BinaryExpr>(
        new BinaryExpr(token, std::move(left), std::move(right)));
  }

  virtual int getPrecedence() const override { return mPrecedence; }

private:
  const int mPrecedence;
  const bool mIsRight;
};
