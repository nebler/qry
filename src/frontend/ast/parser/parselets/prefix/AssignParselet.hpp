#pragma once

#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include "frontend/ast/parser/qryParser.hpp"
class AssignParselet : public InfixParselet {
public:
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      std::unique_ptr<Expr> left,
                                      Token /*token*/) const override {
    std::unique_ptr<Expr> right = parser.parseExpression(ASSIGNMENT - 1);

    IdentifierExpr *leftNameExp = dynamic_cast<IdentifierExpr *>(left.get());
    // if (!leftNameExp) {
    //   throw //
    // }

    std::string name = leftNameExp->identifier;
    return std::unique_ptr<AssignExpr>(new AssignExpr(name, std::move(right)));
  }

  virtual int getPrecedence() const override { return ASSIGNMENT; }
};