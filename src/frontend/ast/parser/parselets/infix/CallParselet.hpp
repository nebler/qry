#pragma once
/**
 * Parselet to parse a function call like "a(b, c, d)".
 */
#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/Parser.hpp"

#include "frontend/ast/parser/Precedence.hpp"
#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include <stdexcept>
#include <utility>

class CallParselet : public InfixParselet {
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      std::unique_ptr<Expr> left,
                                      Token /*token*/) const override {
    std::cout << "lalal" << std::endl;
    if (left->kind() != ExprKind::Identifier) {
      throw std::runtime_error("we expected an identifer expression here");
    }
    auto idExpr = static_cast<const IdentifierExpr *>(left.get());
    std::vector<std::unique_ptr<Expr>> args;
    // There may be no arguments at all.
    if (!parser.match(tok_right_paren)) {
      do {
        args.push_back(parser.parseExpression());
        parser.consume();
      } while (parser.match(tok_comma));
    }

    return std::unique_ptr<CallExpr>(
        new CallExpr(std::move(idExpr->identifier), std::move(args)));
  }

  virtual int getPrecedence() const override { return CALL; }
};