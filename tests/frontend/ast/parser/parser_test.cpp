#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"

#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

TEST(Parser, sum) {

  std::ifstream file("resources/sum.qry");
  ASSERT_TRUE(file.good()) << "File ./resources/sum.qry not found!";
  Lexer lexer = Lexer(file);

  qryParser parser = qryParser(&lexer);

  std::vector<std::unique_ptr<Expr>> exprs = parser.parse();
  std::unique_ptr<IdentifierExpr> a =
      std::unique_ptr<IdentifierExpr>(new IdentifierExpr("a"));

  std::unique_ptr<IdentifierExpr> b =
      std::unique_ptr<IdentifierExpr>(new IdentifierExpr("b"));
  PlusBinaryExpr exprCompare = PlusBinaryExpr(std::move(a), std::move(b));

  for (auto &expr : exprs) {
    EXPECT_EQ(expr->print(), exprCompare.print());
  }
}