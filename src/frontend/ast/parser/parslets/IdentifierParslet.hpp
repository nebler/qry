#pragma once

/**
 * Simple parselet for a named variable like "abc".
 */
#include "frontend/ast/parser/parslets/PrefixParselet.hpp"

class NameParselet : public PrefixParselet {
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      Token token) const override {
    return std::unique_ptr<IdentifierExpr>(
        new IdentifierExpr(parser.getIndentifierStr()));
  }
};
