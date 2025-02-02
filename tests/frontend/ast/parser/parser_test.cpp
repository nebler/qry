#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"

#include <cstddef>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

void testTree(std::vector<std::unique_ptr<Expr>> exprsCompare,
              std::string fileName) {
  std::ifstream file(fileName);
  ASSERT_TRUE(file.good()) << "File ./resources/sum.qry not found!";
  Lexer lexer = Lexer(file);
  qryParser parser = qryParser(&lexer);
  std::vector<std::unique_ptr<Expr>> exprs = parser.parse();
  for (size_t i = 0; i < exprs.size(); i++) {
    EXPECT_EQ(exprs[i]->print(), exprsCompare[i]->print());
  }
}

TEST(Parser, sum) {

  std::ifstream file("resources/sum.qry");
  ASSERT_TRUE(file.good()) << "File ./resources/sum.qry not found!";
  Lexer lexer = Lexer(file);

  qryParser parser = qryParser(&lexer);

  std::vector<std::unique_ptr<Expr>> exprs;
  std::unique_ptr<IdentifierExpr> a =
      std::unique_ptr<IdentifierExpr>(new IdentifierExpr("a"));

  std::unique_ptr<IdentifierExpr> b =
      std::unique_ptr<IdentifierExpr>(new IdentifierExpr("b"));
  exprs.push_back(std::make_unique<PlusBinaryExpr>(std::move(a), std::move(b)));

  testTree(std::move(exprs), "resources/sum.qry");
}
