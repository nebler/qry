#pragma once
#include "frontend/ast/parser/Parser.hpp"

#include <memory>
class Parser;
class InfixParselet {
public:
  virtual ~InfixParselet() = default;
  virtual std::unique_ptr<Expr>
  parse(Parser &parser, std::unique_ptr<Expr> left, Token token) const = 0;
  virtual int getPrecedence() const = 0;
};