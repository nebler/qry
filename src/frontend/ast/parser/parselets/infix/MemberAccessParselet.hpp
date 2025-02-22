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

class MemberAcessParselet : public InfixParselet {
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      std::unique_ptr<Expr> left,
                                      Token token) const override {
    if (left->kind() != ExprKind::Identifier) {
      throw std::runtime_error("we expected an identifer expression here");
    }

    auto idExpr = static_cast<const IdentifierExpr *>(left.get());
    Token name = parser.consume();

    return std::unique_ptr<StructAccessExpr>(
        new StructAccessExpr(idExpr->identifier, name.getText()));
  }

  virtual int getPrecedence() const override { return MEMBER_ACCESS; }
};