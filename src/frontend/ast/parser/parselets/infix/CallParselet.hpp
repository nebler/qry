#pragma once
/**
 * Parselet to parse a function call like "a(b, c, d)".
 */
#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/Parser.hpp"

#include "frontend/ast/parser/Precedence.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include <utility>

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
        new CallExpr(std::move(left), std::move(args)));
  }

  virtual int getPrecedence() const override { return CALL; }
};