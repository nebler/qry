#pragma once

#include "frontend/ast/parser/expression/Expr.hpp"
#include <string>

enum class StmtKind {
  VarDeclaration,
  Expression // We'll need this for expression statements
};

struct Stmt {
  virtual ~Stmt() = default;
  [[nodiscard]] virtual auto kind() const -> StmtKind = 0;
  virtual std::string print() const = 0;
};

struct VarDeclarationStmt : Stmt {
  std::string name;
  std::unique_ptr<Expr> initializer;

  VarDeclarationStmt(std::string name, std::unique_ptr<Expr> init)
      : name(std::move(name)), initializer(std::move(init)) {}

  [[nodiscard]] auto kind() const -> StmtKind override {
    return StmtKind::VarDeclaration;
  }

  std::string print() const override {
    return "VarDeclarationStmt: var " + name + " = " + initializer->print();
  }
};

struct ExpressionStmt : Stmt {
  std::unique_ptr<Expr> expression;

  explicit ExpressionStmt(std::unique_ptr<Expr> expr)
      : expression(std::move(expr)) {}

  [[nodiscard]] auto kind() const -> StmtKind override {
    return StmtKind::Expression;
  }

  std::string print() const override {
    return "ExpressionStmt: " + expression->print();
  }
};
