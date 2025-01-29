#pragma once
#include "frontend/ast/parser/Parser.hpp"
#include <memory>
class Parser;
class PrefixParselet {
public:
  virtual ~PrefixParselet() = default;
  virtual std::unique_ptr<Expr> parse(Parser &parser, Token token) const = 0;
};