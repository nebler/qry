#pragma once

#include <memory>
#include "../Parser.hpp"

class Parser;
// For Tokens that just have a prefix a,4,-4
class PrefixParselet {
public:
  virtual ~PrefixParselet() = default;
  virtual std::unique_ptr<Expr> parse(Parser &parser, Token token) const = 0;
};
