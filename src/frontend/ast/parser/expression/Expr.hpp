#include "frontend/ast/lexer/token/Token.hpp"
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
  Identifier
};

struct Expr {
  [[nodiscard]] virtual auto kind() const -> ExprKind = 0;
  virtual ~Expr() = default;
  virtual std::string print() const = 0;
};

struct AssignExpr : Expr {
  std::string name;
  std::unique_ptr<Expr> right;
  AssignExpr(std::string name, std::unique_ptr<Expr> right)
      : name(std::move(name)), right(std::move(right)) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Assignment;
  }

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

  std::string print() const override {
    std::ostringstream oss;
    oss << "FunctionExpr: call " << name << " with ";
    for (const auto &arg : args) {
      oss << arg->print() << ", ";
    }
    return oss.str() + "\n";
  }
};

struct NumberExpr : Expr {
  double value;
  explicit NumberExpr(double val) : value(val) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Number;
  }

  std::string print() const override {
    return "NumberExpr: " + std::to_string(value) + "\n";
  }
};

struct BinaryExpr : Expr {
  std::string op;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;
  BinaryExpr(std::string op, std::unique_ptr<Expr> lhs,
             std::unique_ptr<Expr> rhs)
      : op(std::move(op)), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Binary;
  }

  std::string print() const override {
    return "BinaryExpr: (" + lhs->print() + " " + op + " " + rhs->print() +
           ")\n";
  }
};

struct PrefixExpr : Expr {
  std::unique_ptr<Expr> right;
  Token op;
  PrefixExpr(Token op, std::unique_ptr<Expr> right)
      : op(op), right(std::move(right)) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Prefix;
  }

  std::string print() const override {
    return "PrefixExpr: (" + op.getText() + " " + right->print() + ")\n";
  }
};

struct CallExpr : Expr {
  std::unique_ptr<Expr> callee;
  std::vector<std::unique_ptr<Expr>> args;
  CallExpr(std::unique_ptr<Expr> callee,
           std::vector<std::unique_ptr<Expr>> args)
      : callee(std::move(callee)), args(std::move(args)) {}

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

struct IdentifierExpr : Expr {
  std::string identifier;
  IdentifierExpr(std::string identifier) : identifier(std::move(identifier)) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Identifier;
  }

  std::string print() const override {
    return "IdentifierExpr: " + identifier + "\n";
  }
};

struct PostfixExpr : Expr {
  std::unique_ptr<Expr> left;
  int operatorType;
  PostfixExpr(std::unique_ptr<Expr> left, int operatorType)
      : left(std::move(left)), operatorType(operatorType) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Call;
  }

  std::string print() const override {
    return "PostfixExpr: (" + left->print() + " operator " +
           std::to_string(operatorType) + ")\n";
  }
};
