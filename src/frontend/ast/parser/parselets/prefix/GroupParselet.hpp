#pragma once

/**
 * Parses parentheses used to group an expression, like "a * (b + c)".
 */
#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
class GroupParselet : public PrefixParselet {
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      Token /*token*/) const override {
    std::unique_ptr<Expr> expression = parser.parseExpression();
    parser.consume(tok_right_paren);
    return expression;
  }
};