#pragma once

#include "../Parser.hpp"
#include <memory>

// For tokens that appear in the middle
class InfixParselet {
public:
  virtual ~InfixParselet() = default;
  virtual std::unique_ptr<Expr>
  parse(Parser &parser, std::unique_ptr<Expr> left, int token) const = 0;
  virtual int getPrecedence() const = 0;
};
