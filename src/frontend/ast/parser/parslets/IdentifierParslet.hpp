#pragma once

/**
 * Simple parselet for a named variable like "abc".
 */
#include "frontend/ast/parser/parslets/PrefixParselet.hpp"

class IdentifierParslet : public PrefixParselet {
  std::unique_ptr<Expr> parse(Parser &parser, int token) const override {
    return std::unique_ptr<IdentifierExpr>(
        new IdentifierExpr(parser.getIndentifierStr()));
  }
};
