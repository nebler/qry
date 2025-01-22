#pragma once

#include "./InfixParslet.hpp"
#include <list>

class CallParselet : public InfixParselet {
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      std::unique_ptr<Expr> left,
                                      Token /*token*/) const override {
    // Parse the comma-separated arguments until we hit, ")".
    std::vector<std::unique_ptr<Expr>> args;

    // There may be no arguments at all.
    if (!parser.match(tok_right_paren)) {
      do {
        args.push_back(parser.parseExpression());
      } while (parser.match(tok_comma));
      parser.consume(tok_right_paren);
    }
    return std::unique_ptr<CallExpr>(
        new CallExpr(parser.getIndentifierStr(), args));
  }

  virtual int getPrecedence() const override { return precedence::CALL; }
};