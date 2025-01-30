
#include "frontend/ast/lexer/token/Token.hpp"
#include <memory>
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
  // Virtual method to get the kind of expression
  [[nodiscard]] virtual auto kind() const -> ExprKind = 0;

  virtual ~Expr() = default;
  // Helper method for safe downcasting
  template <typename T> [[nodiscard]] const T &as() const {
    return static_cast<const T &>(*this);
  }
};

struct AssignExpr : Expr {
  std::string name;
  std::unique_ptr<Expr> right;
  AssignExpr(std::string name, std::unique_ptr<Expr> right)
      : name(std::move(name)), right(std::move(right)) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Assignment;
  }
};
struct FunctionExpr : Expr {
  std::string name;
  std::vector<Expr> args;
  FunctionExpr(std::string name, std::vector<Expr> args)
      : name(std::move(name)), args(std::move(args)) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Function;
  }
};

struct NumberExpr : Expr {
  double value;
  explicit NumberExpr(double val) : value(val) {}
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Number;
  }
};

struct BinaryExpr : Expr {
  std::string op;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;
  BinaryExpr(std::string op, std::unique_ptr<Expr> lhs,
             std::unique_ptr<Expr> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Binary;
  }
};

struct PrefixExpr : Expr {
  std::unique_ptr<Expr> mRight;
  Token mOperator;
  PrefixExpr(Token operatorType, std::unique_ptr<Expr> right)
      : mOperator(operatorType), mRight(std::move(right)) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Prefix;
  }
};
struct CallExpr : Expr {
  std::string callee;
  std::vector<std::unique_ptr<Expr>> args;
  CallExpr(std::string callee, std::vector<std::unique_ptr<Expr>> args)
      : callee(std::move(callee)), args(std::move(args)) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Call;
  }
};
struct IdentifierExpr : Expr {
  std::string identifier;
  IdentifierExpr(std::string identifier) : identifier(identifier) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Identifier;
  }
};
struct PostfixExpr : Expr {
  std::unique_ptr<Expr> left;
  int operatorType;
  PostfixExpr(std::unique_ptr<Expr> left, int operatorType)
      : left(std::move(left)), operatorType(operatorType) {};
  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Call;
  }
};