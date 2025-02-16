#pragma once

#include "frontend/ast/parser/Parser.hpp"

#include <memory>

class StatementParselet {
public:
  virtual ~StatementParselet() = default;
  virtual std::unique_ptr<Stmt> parse(Parser &parser, Token token) const = 0;
};
