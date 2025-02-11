#pragma once

// Now let's create our visitor that builds the symbol table
#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/symboltable/SymbolTable.hpp"
class SymbolTableBuilder : public ASTVisitor {
private:
  SymbolTable &symbolTable;

  // Helper to visit all expressions in a vector
  void visitExpressions(const std::vector<std::unique_ptr<Expr>> &exprs) {
    for (const auto &expr : exprs) {
      expr->accept(*this);
    }
  }

public:
  explicit SymbolTableBuilder(SymbolTable &st) : symbolTable(st) {}

  // Visit variable declarations
  void visitVarDeclarationStmt(const VarDeclarationStmt *stmt) override {
    // First visit the initializer to catch any variables used there
    stmt->initializer->accept(*this);

    // Now declare this variable
    symbolTable.declare(stmt->name, stmt);

    // Since it has an initializer, mark it as defined
    symbolTable.define(stmt->name);
  }

  // Visit identifiers to record variable uses
  void visitIdentifierExpr(const IdentifierExpr *expr) override {
    // Record this usage of the variable
    symbolTable.addUse(expr->identifier, expr);
  }

  // Visit binary expressions
  void visitBinaryExpr(const BinaryExpr *expr) override {
    // Visit both sides to catch any variables
    expr->lhs->accept(*this);
    expr->rhs->accept(*this);
  }

  void visitPlusBinaryExpr(const PlusBinaryExpr *expr) override {
    visitBinaryExpr(expr);
  }

  void visitNumberExpr(const NumberExpr * /*expr*/) override {
    // Nothing to do for numbers
  }

  void visitPrefixExpr(const PrefixExpr *expr) override {
    // Visit the operand
    expr->right->accept(*this);
  }

  void visitCallExpr(const CallExpr *expr) override {
    // Visit the function being called
    expr->callee->accept(*this);

    // Visit all arguments
    visitExpressions(expr->args);
  }

  void visitExpressionStmt(const ExpressionStmt *stmt) override {
    // Just visit the expression
    stmt->expression->accept(*this);
  }
};