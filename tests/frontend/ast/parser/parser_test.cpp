#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

// Helper function to create test files with given content
void createTestFile(const std::string &filename, const std::string &content) {
  std::ofstream file(filename);
  file << content;
  file.close();
}

// Test helper function (same as provided)
void testTree(std::vector<std::unique_ptr<Expr>> exprsCompare,
              std::string fileName) {
  std::ifstream file(fileName);
  ASSERT_TRUE(file.good()) << "File " << fileName << " not found!";
  Lexer lexer = Lexer(file);
  qryParser parser = qryParser(&lexer);
  std::vector<std::unique_ptr<Expr>> exprs = parser.parse();
  for (size_t i = 0; i < exprs.size(); i++) {
    EXPECT_EQ(exprs[i]->print(), exprsCompare[i]->print());
  }
}

// Test case for simple addition: a+b
TEST(Parser, SimpleAddition) {
  createTestFile("resources/simple_add.qry", "a+b");

  std::vector<std::unique_ptr<Expr>> exprs;
  auto a = std::make_unique<IdentifierExpr>("a");
  auto b = std::make_unique<IdentifierExpr>("b");
  exprs.push_back(std::make_unique<PlusBinaryExpr>(std::move(a), std::move(b)));

  testTree(std::move(exprs), "resources/simple_add.qry");
}

// Test case for simple subtraction: a-b
TEST(Parser, SimpleSubtraction) {
  createTestFile("resources/simple_sub.qry", "a-b");

  std::vector<std::unique_ptr<Expr>> exprs;
  auto a = std::make_unique<IdentifierExpr>("a");
  auto b = std::make_unique<IdentifierExpr>("b");
  exprs.push_back(
      std::make_unique<MinusBinaryExpr>(std::move(a), std::move(b)));

  testTree(std::move(exprs), "resources/simple_sub.qry");
}

// Test case for complex expression with multiplication: a+b*c-d
TEST(Parser, ComplexExpression) {
  createTestFile("resources/complex.qry", "a+b*c-d");

  std::vector<std::unique_ptr<Expr>> exprs;

  // Building the AST from bottom up:
  // First create b*c
  auto b = std::make_unique<IdentifierExpr>("b");
  auto c = std::make_unique<IdentifierExpr>("c");
  auto mult =
      std::make_unique<MultiplicationBinaryExpr>(std::move(b), std::move(c));

  // Then a+(b*c)
  auto a = std::make_unique<IdentifierExpr>("a");
  auto plus = std::make_unique<PlusBinaryExpr>(std::move(a), std::move(mult));

  // Finally (a+(b*c))-d
  auto d = std::make_unique<IdentifierExpr>("d");
  exprs.push_back(
      std::make_unique<MinusBinaryExpr>(std::move(plus), std::move(d)));

  testTree(std::move(exprs), "resources/complex.qry");
}

// Test case for complex expression with multiple operations: d+e-i*c
TEST(Parser, MultipleOperations) {
  createTestFile("resources/multiple_ops.qry", "d+e-i*c");

  std::vector<std::unique_ptr<Expr>> exprs;

  // Building the AST from bottom up:
  // First create i*c
  auto i = std::make_unique<IdentifierExpr>("i");
  auto c = std::make_unique<IdentifierExpr>("c");
  auto mult =
      std::make_unique<MultiplicationBinaryExpr>(std::move(i), std::move(c));

  // Then create d+e
  auto d = std::make_unique<IdentifierExpr>("d");
  auto e = std::make_unique<IdentifierExpr>("e");
  auto plus = std::make_unique<PlusBinaryExpr>(std::move(d), std::move(e));

  // Finally (d+e)-(i*c)
  exprs.push_back(
      std::make_unique<MinusBinaryExpr>(std::move(plus), std::move(mult)));

  testTree(std::move(exprs), "resources/multiple_ops.qry");
}

// Generic helper function for creating test files with multiple expressions
void createMultiExpressionFile(const std::string &filename,
                               const std::vector<std::string> &expressions) {
  std::ofstream file(filename);
  for (size_t i = 0; i < expressions.size(); ++i) {
    file << expressions[i];
    if (i < expressions.size() - 1) {
      file << '\n';
    }
  }
  file.close();
}

TEST(Parser, AllExpressions) {
  // Our specific test expressions
  const std::vector<std::string> expressions = {"a+b*c-d", "a+b", "a-b",
                                                "d+e-i*c"};

  // Create test file using our generic function
  const std::string filename = "resources/all_expressions.qry";
  createMultiExpressionFile(filename, expressions);

  // Create the expected AST for all expressions
  std::vector<std::unique_ptr<Expr>> expectedExprs;

  // 1. Build AST for "a+b*c-d"
  {
    // Create b*c
    auto b1 = std::make_unique<IdentifierExpr>("b");
    auto c1 = std::make_unique<IdentifierExpr>("c");
    auto mult = std::make_unique<MultiplicationBinaryExpr>(std::move(b1),
                                                           std::move(c1));

    // Create a+(b*c)
    auto a1 = std::make_unique<IdentifierExpr>("a");
    auto plus =
        std::make_unique<PlusBinaryExpr>(std::move(a1), std::move(mult));

    // Create (a+(b*c))-d
    auto d1 = std::make_unique<IdentifierExpr>("d");
    expectedExprs.push_back(
        std::make_unique<MinusBinaryExpr>(std::move(plus), std::move(d1)));
  }

  // 2. Build AST for "a+b"
  {
    auto a2 = std::make_unique<IdentifierExpr>("a");
    auto b2 = std::make_unique<IdentifierExpr>("b");
    expectedExprs.push_back(
        std::make_unique<PlusBinaryExpr>(std::move(a2), std::move(b2)));
  }

  // 3. Build AST for "a-b"
  {
    auto a3 = std::make_unique<IdentifierExpr>("a");
    auto b3 = std::make_unique<IdentifierExpr>("b");
    expectedExprs.push_back(
        std::make_unique<MinusBinaryExpr>(std::move(a3), std::move(b3)));
  }

  // 4. Build AST for "d+e-i*c"
  {
    // Create i*c
    auto i = std::make_unique<IdentifierExpr>("i");
    auto c4 = std::make_unique<IdentifierExpr>("c");
    auto mult =
        std::make_unique<MultiplicationBinaryExpr>(std::move(i), std::move(c4));

    // Create d+e
    auto d4 = std::make_unique<IdentifierExpr>("d");
    auto e = std::make_unique<IdentifierExpr>("e");
    auto plus = std::make_unique<PlusBinaryExpr>(std::move(d4), std::move(e));

    // Create (d+e)-(i*c)
    expectedExprs.push_back(
        std::make_unique<MinusBinaryExpr>(std::move(plus), std::move(mult)));
  }

  // Parse and verify all expressions
  std::ifstream file(filename);
  ASSERT_TRUE(file.good()) << "File " << filename << " not found!";
  Lexer lexer = Lexer(file);
  qryParser parser = qryParser(&lexer);
  std::vector<std::unique_ptr<Expr>> actualExprs = parser.parse();

  // Verify we got the expected number of expressions
  ASSERT_EQ(actualExprs.size(), expectedExprs.size())
      << "Number of parsed expressions doesn't match expected count";

  // Compare each expression
  for (size_t i = 0; i < actualExprs.size(); i++) {
    EXPECT_EQ(actualExprs[i]->print(), expectedExprs[i]->print())
        << "Mismatch in expression " << i + 1;
  }
}