#pragma once
#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/symboltable/Scope.hpp"
#include <cstddef>
#include <memory>

class SymbolTableBuilder : public ASTVisitor {
private:
  std::unique_ptr<Scope> currentScope;

  void visitExpressions(const std::vector<std::unique_ptr<Expr>> &exprs) {
    for (const auto &expr : exprs) {
      expr->accept(*this);
    }
  }

  Scope *enterScope(ScopeType type) {
    auto newScope = std::make_unique<Scope>(currentScope.get(), type);
    Scope *ptr = newScope.get();

    if (currentScope) {
      currentScope->addChild(std::move(newScope));
    } else {
      currentScope = std::move(newScope);
    }
    return ptr;
  }

  std::optional<Symbol *> getSymbol(const std::string &name) {
    Scope *scope = currentScope.get();
    while (scope) {
      if (Symbol *symbol = scope->lookupLocal(name)) {
        return symbol;
      }
      scope = scope->getParent();
    }
    return std::nullopt;
  }

public:
  static std::unique_ptr<Scope>
  createSymbolTable(const std::unique_ptr<ProgramNode> &ast) {
    SymbolTableBuilder builder;
    builder.currentScope = std::make_unique<Scope>(nullptr, ScopeType::Global);
    for (const auto &node : ast->statements) {
      node->accept(builder);
    }
    return std::move(builder.currentScope);
  }

  void visitVarDeclarationStmt(const VarDeclarationStmt *stmt) override {

    stmt->initializer->accept(*this);

    currentScope->declare(stmt->name, stmt);
  }

  void visitIdentifierExpr(const IdentifierExpr *expr) override {
    if (auto symbolOpt = getSymbol(expr->identifier)) {
      Symbol *symbol = *symbolOpt;
      symbol->addUse(expr);
    } else {
      throw std::runtime_error("Use of undeclared variable '" +
                               expr->identifier + "'");
    }
  }

  void visitBinaryExpr(const BinaryExpr *expr) override {
    expr->lhs->accept(*this);
    expr->rhs->accept(*this);
  }

  void visitPlusBinaryExpr(const PlusBinaryExpr *expr) override {
    visitBinaryExpr(expr);
  }

  virtual void visitMinusBinaryExpr(const MinusBinaryExpr *expr) override {
    visitBinaryExpr(expr);
  }

  virtual void
  visitDivisionBinaryExpr(const DivisionBinaryExpr *expr) override {
    visitBinaryExpr(expr);
  }

  virtual void
  visitExponentBinaryExpr(const ExponentBinaryExpr *expr) override {
    visitBinaryExpr(expr);
  }

  virtual void
  visitMultiplicationBinaryExpr(const MultiplicationBinaryExpr *expr) override {
    visitBinaryExpr(expr);
  }

  void visitIntExpr(const IntExpr *expr) override {
    // Nothing to do for numbers
  }

  void visitFloatExpr(const FloatExpr *expr) override {
    // Nothing to do for numbers
  }

  void visitStringExpr(const StringExpr *expr) override {
    // Nothing to do for string
  }

  void visitBoolExpr(const BoolExpr *exprs) override {
    // Nothing to do for bool
  }

  void visitPrefixExpr(const PrefixExpr *expr) override {
    expr->right->accept(*this);
  }

  void visitCallExpr(const CallExpr *expr) override {
    // Visit the function being called
    expr->callee->accept(*this);

    // Visit all arguments
    visitExpressions(expr->args);
  }

  virtual void visitAssignExpr(const AssignExpr *expr) override {
    if (auto symbolOpt = getSymbol(expr->name)) {
      Symbol *symbol = *symbolOpt;
    } else {
      throw std::runtime_error("Use of undeclared variable '" + expr->name +
                               "'");
    }
  }
  virtual void visitFunctionExpr(const FunctionExpr *expr) override {}
  virtual void visitExpressionStmt(const ExpressionStmt *stmt) override {}
};