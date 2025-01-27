#pragma once

/**
 * Simple parselet for a named variable like "abc".
 */

#include <memory>
#include "PrefixParselet.hpp"
class IdentifierParslet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, Token token) const override {
    return std::unique_ptr<IdentifierExpr>(
        new IdentifierExpr(parser.getIndentifierStr()));
  }
};
