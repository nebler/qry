#pragma once
#include "frontend/ast/lexer/token/Token.hpp"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// First, we define our kind enums
enum class ExprKind {
  Number,
  Variable,
  Binary,
  Call,
  Function,
  Prototype,
  Assignment,
  Struct,
  Prefix,
  Identifier,
  BinaryPlus,
  BinaryMinus,
  BinaryDivision,
  BinaryExponent,
  BinaryMultiplication
};

enum class StmtKind { VarDeclaration, Expression };

// Forward declare visitor
class ASTVisitor;
// Forward declare Token
class Token;

// Our base AST node - everything inherits from this
struct ASTNode {
  virtual ~ASTNode() = default;
  virtual void accept(ASTVisitor &visitor) = 0;
  virtual std::string print() const = 0;
};

// Base class for all expressions
struct Expr : ASTNode {
  [[nodiscard]] virtual auto kind() const -> ExprKind = 0;
};

// Base class for all statements
struct Stmt : ASTNode {
  [[nodiscard]] virtual auto kind() const -> StmtKind = 0;
};

// Now for our concrete expression classes
struct NumberExpr : Expr {
  double value;

  explicit NumberExpr(double val) : value(val) {}

  void accept(ASTVisitor &visitor) override; // Defined after visitor class

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Number;
  }

  std::string print() const override {
    return "NumberExpr: " + std::to_string(value) + "\n";
  }
};

struct IdentifierExpr : Expr {
  std::string identifier;

  explicit IdentifierExpr(std::string id) : identifier(std::move(id)) {}

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Identifier;
  }

  std::string print() const override {
    return "IdentifierExpr: " + identifier + "\n";
  }
};

struct AssignExpr : Expr {
  std::string name;
  std::unique_ptr<Expr> right;
  AssignExpr(std::string name, std::unique_ptr<Expr> right)
      : name(std::move(name)), right(std::move(right)) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Assignment;
  }
  void accept(ASTVisitor &visitor) override;

  std::string print() const override {
    return "AssignExpr: assign " + name + " to " + right->print() + "\n";
  }
};

struct FunctionExpr : Expr {
  std::string name;
  std::vector<std::unique_ptr<Expr>> args;
  FunctionExpr(std::string name, std::vector<std::unique_ptr<Expr>> args)
      : name(std::move(name)), args(std::move(args)) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Function;
  }

  void accept(ASTVisitor &visitor);

  std::string print() const override {
    std::ostringstream oss;
    oss << "FunctionExpr: call " << name << " with ";
    for (const auto &arg : args) {
      oss << arg->print() << ", ";
    }
    return oss.str() + "\n";
  }
};

struct BinaryExpr : Expr {
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;

  BinaryExpr(std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Binary;
  }

  virtual std::string print() const override = 0;
};

struct PlusBinaryExpr : BinaryExpr {
  using BinaryExpr::BinaryExpr;

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::BinaryPlus;
  }

  std::string print() const override {
    return "BinaryExpr: \n" + lhs->print() + " + " + rhs->print() + "\n";
  }
};

struct MinusBinaryExpr : BinaryExpr {
  using BinaryExpr::BinaryExpr;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::BinaryMinus;
  }

  void accept(ASTVisitor &visitor) override;

  std::string print() const override {
    return "BinaryExpr: \n" + lhs->print() + " - " + rhs->print() + "\n";
  }
};

struct DivisionBinaryExpr : BinaryExpr {
  using BinaryExpr::BinaryExpr;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::BinaryDivision;
  }

  void accept(ASTVisitor &visitor) override;

  std::string print() const override {
    return "BinaryExpr: \n" + lhs->print() + " / " + rhs->print() + "\n";
  }
};

struct ExponentBinaryExpr : BinaryExpr {
  using BinaryExpr::BinaryExpr;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::BinaryExponent;
  }

  void accept(ASTVisitor &visitor) override;

  std::string print() const override {
    return "BinaryExpr: \n" + lhs->print() + " ^ " + rhs->print() + "\n";
  }
};

struct MultiplicationBinaryExpr : BinaryExpr {
  using BinaryExpr::BinaryExpr;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::BinaryMultiplication;
  }

  void accept(ASTVisitor &visitor) override;

  std::string print() const override {
    return "BinaryExpr: \n" + lhs->print() + " * " + rhs->print() + "\n";
  }
};

// Similar for other binary expressions...

struct PrefixExpr : Expr {
  std::unique_ptr<Expr> right;
  Token op;

  PrefixExpr(Token op, std::unique_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Prefix;
  }

  std::string print() const override {
    return "PrefixExpr: \n" + op.getText() + " " + right->print() + "\n";
  }
};

struct CallExpr : Expr {
  std::unique_ptr<Expr> callee;
  std::vector<std::unique_ptr<Expr>> args;

  CallExpr(std::unique_ptr<Expr> callee,
           std::vector<std::unique_ptr<Expr>> args)
      : callee(std::move(callee)), args(std::move(args)) {}

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Call;
  }

  std::string print() const override {
    std::ostringstream oss;
    oss << "CallExpr: " << callee->print() << "(";
    for (const auto &arg : args) {
      oss << arg->print() << ", ";
    }
    return oss.str() + ")\n";
  }
};

// Now for our concrete statement classes
struct VarDeclarationStmt : Stmt {
  std::string name;
  std::unique_ptr<Expr> initializer;

  VarDeclarationStmt(std::string name, std::unique_ptr<Expr> init)
      : name(std::move(name)), initializer(std::move(init)) {}

  void accept(ASTVisitor &visitor) override;

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

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> StmtKind override {
    return StmtKind::Expression;
  }

  std::string print() const override {
    return "ExpressionStmt: " + expression->print();
  }
};
class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;

  // Expression visitors
  virtual void visitNumberExpr(const NumberExpr *expr) = 0;
  virtual void visitIdentifierExpr(const IdentifierExpr *expr) = 0;
  virtual void visitAssignExpr(const AssignExpr *expr) = 0;
  virtual void visitFunctionExpr(const FunctionExpr *expr) = 0;
  virtual void visitBinaryExpr(const BinaryExpr *expr) = 0;
  virtual void visitPlusBinaryExpr(const PlusBinaryExpr *expr) = 0;
  virtual void visitMinusBinaryExpr(const MinusBinaryExpr *expr) = 0;
  virtual void visitDivisionBinaryExpr(const DivisionBinaryExpr *expr) = 0;
  virtual void visitExponentBinaryExpr(const ExponentBinaryExpr *expr) = 0;
  virtual void
  visitMultiplicationBinaryExpr(const MultiplicationBinaryExpr *expr) = 0;
  virtual void visitPrefixExpr(const PrefixExpr *expr) = 0;
  virtual void visitCallExpr(const CallExpr *expr) = 0;

  // Statement visitors
  virtual void visitVarDeclarationStmt(const VarDeclarationStmt *stmt) = 0;
  virtual void visitExpressionStmt(const ExpressionStmt *stmt) = 0;
};

// Now implement the accept methods for each node type after the ASTVisitor
// class:

void NumberExpr::accept(ASTVisitor &visitor) { visitor.visitNumberExpr(this); }

void IdentifierExpr::accept(ASTVisitor &visitor) {
  visitor.visitIdentifierExpr(this);
}

void AssignExpr::accept(ASTVisitor &visitor) { visitor.visitAssignExpr(this); }

void FunctionExpr::accept(ASTVisitor &visitor) {
  visitor.visitFunctionExpr(this);
}

void BinaryExpr::accept(ASTVisitor &visitor) { visitor.visitBinaryExpr(this); }

void PlusBinaryExpr::accept(ASTVisitor &visitor) {
  visitor.visitPlusBinaryExpr(this);
}

void MinusBinaryExpr::accept(ASTVisitor &visitor) {
  visitor.visitMinusBinaryExpr(this);
}

void DivisionBinaryExpr::accept(ASTVisitor &visitor) {
  visitor.visitDivisionBinaryExpr(this);
}

void ExponentBinaryExpr::accept(ASTVisitor &visitor) {
  visitor.visitExponentBinaryExpr(this);
}

void MultiplicationBinaryExpr::accept(ASTVisitor &visitor) {
  visitor.visitMultiplicationBinaryExpr(this);
}

void PrefixExpr::accept(ASTVisitor &visitor) { visitor.visitPrefixExpr(this); }

void CallExpr::accept(ASTVisitor &visitor) { visitor.visitCallExpr(this); }

void VarDeclarationStmt::accept(ASTVisitor &visitor) {
  visitor.visitVarDeclarationStmt(this);
}

void ExpressionStmt::accept(ASTVisitor &visitor) {
  visitor.visitExpressionStmt(this);
}
