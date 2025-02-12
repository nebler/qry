#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/ast/ASTNode.hpp"
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
// First, update the test helper function to handle statements
void testTree(std::vector<std::unique_ptr<Stmt>> stmtsCompare,
              std::string fileName) {
  std::ifstream file(fileName);
  ASSERT_TRUE(file.good()) << "File " << fileName << " not found!";
  Lexer lexer = Lexer(file);
  qryParser parser = qryParser(&lexer);
  std::unique_ptr<ProgramNode> astTree = parser.parse();
  for (size_t i = 0; i < astTree->statements.size(); i++) {
    EXPECT_EQ(astTree->statements[i]->print(), stmtsCompare[i]->print());
  }
}

// Test case for simple variable declaration with addition
TEST(Parser, SimpleAddition) {
  createTestFile("resources/simple_add.qry", "var a = 1 + 2;");

  std::vector<std::unique_ptr<Stmt>> stmts;
  auto num1 = std::make_unique<NumberExpr>(1);
  auto num2 = std::make_unique<NumberExpr>(2);
  auto plus =
      std::make_unique<PlusBinaryExpr>(std::move(num1), std::move(num2));
  stmts.push_back(std::make_unique<VarDeclarationStmt>("a", std::move(plus)));

  testTree(std::move(stmts), "resources/simple_add.qry");
}

// Test case for simple subtraction
TEST(Parser, SimpleSubtraction) {
  createTestFile("resources/simple_sub.qry", "var b = 5 - 3;");

  std::vector<std::unique_ptr<Stmt>> stmts;
  auto num1 = std::make_unique<NumberExpr>(5);
  auto num2 = std::make_unique<NumberExpr>(3);
  auto minus =
      std::make_unique<MinusBinaryExpr>(std::move(num1), std::move(num2));
  stmts.push_back(std::make_unique<VarDeclarationStmt>("b", std::move(minus)));

  testTree(std::move(stmts), "resources/simple_sub.qry");
}

// Test case for complex expression with multiplication
TEST(Parser, ComplexExpression) {
  createTestFile("resources/complex.qry", "var result = 2 + 3 * 4 - 1;");

  std::vector<std::unique_ptr<Stmt>> stmts;

  // Building the AST from bottom up:
  // First create 3*4
  auto num3 = std::make_unique<NumberExpr>(3);
  auto num4 = std::make_unique<NumberExpr>(4);
  auto mult = std::make_unique<MultiplicationBinaryExpr>(std::move(num3),
                                                         std::move(num4));

  // Then 2+(3*4)
  auto num2 = std::make_unique<NumberExpr>(2);
  auto plus =
      std::make_unique<PlusBinaryExpr>(std::move(num2), std::move(mult));

  // Finally (2+(3*4))-1
  auto num1 = std::make_unique<NumberExpr>(1);
  auto minus =
      std::make_unique<MinusBinaryExpr>(std::move(plus), std::move(num1));

  stmts.push_back(
      std::make_unique<VarDeclarationStmt>("result", std::move(minus)));

  testTree(std::move(stmts), "resources/complex.qry");
}

// Test case for variable reference in expression
TEST(Parser, VariableReference) {
  createTestFile("resources/var_ref.qry", "var b = a + 2;");

  std::vector<std::unique_ptr<Stmt>> stmts;
  auto varRef = std::make_unique<IdentifierExpr>("a");
  auto num = std::make_unique<NumberExpr>(2);
  auto plus =
      std::make_unique<PlusBinaryExpr>(std::move(varRef), std::move(num));
  stmts.push_back(std::make_unique<VarDeclarationStmt>("b", std::move(plus)));

  testTree(std::move(stmts), "resources/var_ref.qry");
}

// Test case for multiple variable declarations
TEST(Parser, MultipleDeclarations) {
  const std::vector<std::string> declarations = {
      "var x = 1 + 2;", "var y = 3 * 4;", "var z = x + y;",
      "var result = z - 5;"};

  const std::string filename = "resources/multiple_decls.qry";
  createMultiExpressionFile(filename, declarations);

  std::vector<std::unique_ptr<Stmt>> expectedStmts;

  // First declaration: var x = 1 + 2;
  {
    auto num1 = std::make_unique<NumberExpr>(1);
    auto num2 = std::make_unique<NumberExpr>(2);
    auto plus =
        std::make_unique<PlusBinaryExpr>(std::move(num1), std::move(num2));
    expectedStmts.push_back(
        std::make_unique<VarDeclarationStmt>("x", std::move(plus)));
  }

  // Second declaration: var y = 3 * 4;
  {
    auto num3 = std::make_unique<NumberExpr>(3);
    auto num4 = std::make_unique<NumberExpr>(4);
    auto mult = std::make_unique<MultiplicationBinaryExpr>(std::move(num3),
                                                           std::move(num4));
    expectedStmts.push_back(
        std::make_unique<VarDeclarationStmt>("y", std::move(mult)));
  }

  // Third declaration: var z = x + y;
  {
    auto varX = std::make_unique<IdentifierExpr>("x");
    auto varY = std::make_unique<IdentifierExpr>("y");
    auto plus =
        std::make_unique<PlusBinaryExpr>(std::move(varX), std::move(varY));
    expectedStmts.push_back(
        std::make_unique<VarDeclarationStmt>("z", std::move(plus)));
  }

  // Fourth declaration: var result = z - 5;
  {
    auto varZ = std::make_unique<IdentifierExpr>("z");
    auto num5 = std::make_unique<NumberExpr>(5);
    auto minus =
        std::make_unique<MinusBinaryExpr>(std::move(varZ), std::move(num5));
    expectedStmts.push_back(
        std::make_unique<VarDeclarationStmt>("result", std::move(minus)));
  }

  testTree(std::move(expectedStmts), filename);
}