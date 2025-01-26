#pragma once

#include "../Parser.hpp"
#include <memory>

// For Tokens that just have a prefix a,4,-4
class PrefixParselet {
public:
  virtual ~PrefixParselet() = default;
  virtual std::unique_ptr<Expr> parse(Parser &parser, int token) const;
  virtual int getPrecedence() const;
};
