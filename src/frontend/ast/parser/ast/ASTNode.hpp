#pragma once
#include "frontend/ast/lexer/token/Token.hpp"
#include "frontend/ast/parser/types/ASTType.hpp"
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

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
  BinaryMultiplication,
  StringExpr,
  IntExpr,
  BoolExpr,
  FloatExpr
};

enum class StmtKind {
  VarDeclaration,
  Expression,
  ProgramNode,
  StructDeclaration
};

class ASTVisitor;
class Token;

struct ASTNode {
  virtual ~ASTNode() = default;
  virtual void accept(ASTVisitor &visitor) = 0;
  virtual std::string print() const = 0;
};

struct Expr : ASTNode {
  ASTType type;
  [[nodiscard]] virtual auto kind() const -> ExprKind = 0;
};

struct Stmt : ASTNode {
  [[nodiscard]] virtual auto kind() const -> StmtKind = 0;
};

struct ProgramNode : Stmt {
  std::vector<std::unique_ptr<Stmt>> statements;

  explicit ProgramNode(std::vector<std::unique_ptr<Stmt>> stmts)
      : statements(std::move(stmts)) {}

  void accept(ASTVisitor &visitor) override {}

  [[nodiscard]] auto kind() const -> StmtKind override {
    return StmtKind::ProgramNode;
  }

  std::string print() const override {
    std::string result = "Program:\n";
    for (const auto &stmt : statements) {
      result += stmt->print() + "\n";
    }
    return result;
  }
};

struct StringExpr : Expr {
  std::string val;
  explicit StringExpr(std::string val) : val(val) { type = ASTType::STRING; }

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::StringExpr;
  }

  std::string print() const override { return "StringExpr: " + val + "\n"; }
};

struct BoolExpr : Expr {
  bool val;
  explicit BoolExpr(bool val) : val(val) { type = ASTType::BOOL; }

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::BoolExpr;
  }

  std::string print() const override {

    if (val) {
      return "BoolExpr: true\n";
    }
    return "BoolExpr: false\n";
  }
};

struct IntExpr : Expr {
  double value;
  explicit IntExpr(int val) : value(val) { type = ASTType::INT; }

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::IntExpr;
  }

  std::string print() const override {
    return "IntExpr: " + std::to_string(value) + "\n";
  }
};

struct FloatExpr : Expr {
  double value;
  explicit FloatExpr(double val) : value(val) { type = ASTType::FLOAT; }

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::FloatExpr;
  }

  std::string print() const override {
    return "FloatExpr: " + std::to_string(value) + "\n";
  }
};

struct IdentifierExpr : Expr {
  std::string identifier;

  explicit IdentifierExpr(std::string id) : identifier(std::move(id)) {
    type = ASTType::UNKNOWN;
  }

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
      : name(std::move(name)), right(std::move(right)) {
    type = ASTType::UNKNOWN;
  }

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
      : name(std::move(name)), args(std::move(args)) {
    type = ASTType::UNKNOWN;
  }

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Function;
  }

  void accept(ASTVisitor &visitor) override;

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
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    type = ASTType::UNKNOWN;
  }

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

struct PrefixExpr : Expr {
  std::unique_ptr<Expr> right;
  Token op;

  PrefixExpr(Token op, std::unique_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {
    type = right->type;
  }

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
      : callee(std::move(callee)), args(std::move(args)) {
    type = ASTType::UNKNOWN;
  }

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

struct VarDeclarationStmt : Stmt {
  std::string name;
  std::unique_ptr<Expr> initializer;
  ASTType type;

  void accept(ASTVisitor &visitor) override;

  VarDeclarationStmt(std::string name, std::unique_ptr<Expr> init, ASTType type)
      : name(std::move(name)), initializer(std::move(init)), type(type) {}

  [[nodiscard]] auto kind() const -> StmtKind override {
    return StmtKind::VarDeclaration;
  }

  std::string print() const override {
    return "VarDeclarationStmt: var " + name + " = " + initializer->print();
  }
};

struct StructDeclarationStmt : Stmt {
  std::string name;
  std::map<std::string, ASTType> members;

  // We don't need type since a struct is its own type
  // The type will be created in the type system based on this declaration

  StructDeclarationStmt(std::string name,
                        std::map<std::string, ASTType> members)
      : name(std::move(name)), members(std::move(members)) {}

  void accept(ASTVisitor &visitor) override;

  [[nodiscard]] auto kind() const -> StmtKind override {
    return StmtKind::StructDeclaration;
  }

  std::string print() const override {
    std::string result = "struct " + name + " {\n";

    // Print each member field with proper indentation
    bool first = true;
    for (const auto &[fieldName, fieldType] : members) {
      if (!first) {
        result += ",\n";
      }
      result += "    " + fieldName + ": " + toString(fieldType);
      first = false;
    }

    // Add closing brace on new line with proper alignment
    result += "\n}";
    return result;
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
  virtual void visitIntExpr(const IntExpr *expr) = 0;
  virtual void visitFloatExpr(const FloatExpr *expr) = 0;
  virtual void visitStringExpr(const StringExpr *expr) = 0;
  virtual void visitBoolExpr(const BoolExpr *expr) = 0;
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
  virtual void visitStructDeclrationStmt(const StructDeclarationStmt *stmt) = 0;
};

void IntExpr::accept(ASTVisitor &visitor) { visitor.visitIntExpr(this); }
void FloatExpr::accept(ASTVisitor &visitor) { visitor.visitFloatExpr(this); }
void BoolExpr::accept(ASTVisitor &visitor) { visitor.visitBoolExpr(this); }
void StringExpr::accept(ASTVisitor &visitor) { visitor.visitStringExpr(this); }

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

inline void ExponentBinaryExpr::accept(ASTVisitor &visitor) {
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

void StructDeclarationStmt::accept(ASTVisitor &visitor) {
  visitor.visitStructDeclrationStmt(this);
}
