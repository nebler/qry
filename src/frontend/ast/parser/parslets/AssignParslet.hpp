#include "../Precedence.hpp"
#include "../Parser.hpp"
#pragma once

class AssignParselet : public InfixParselet {
public:
  virtual std::unique_ptr<Expr>
  parse(Parser &parser, std::unique_ptr<Expr> left, Token /*token*/) {
    std::unique_ptr<Expr> right =
        parser.parseExpression(precedence::ASSIGNMENT - 1);

    IdentifierExpr *leftNameExp = dynamic_cast<IdentifierExpr *>(left.get());

    std::string name = leftNameExp->identifier;
    return std::unique_ptr<AssignExpr>(new AssignExpr(name, std::move(right)));
  }
};
