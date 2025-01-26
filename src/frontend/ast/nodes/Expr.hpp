#include "../types/Types.hpp"
#include "frontend/ast/lexer/Token.hpp"
#include <memory>
#include <string>
// All possible types of expressions in our AST
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

// Our base expression struct that all others will inherit from
struct Expr {
  // Virtual method to get the kind of expression
  [[nodiscard]] virtual auto kind() const -> ExprKind = 0;

  // Virtual destructor for proper cleanup
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

// Number literals (like 42.0)
struct NumberExpr : Expr {
  double value;

  explicit NumberExpr(double val) : value(val) {}

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Number;
  }
};

struct VariableExpr : Expr {
  std::string name;

  TypeKind type;
  VariableExpr(std::string name, TypeKind type)
      : name(std::move(name)), type(type) {};

  [[nodiscard]] auto kind() const -> ExprKind override {
    return ExprKind::Variable;
  }
};

// Binary operations (like a + b)
struct BinaryExpr : Expr {
  char op;
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;

  BinaryExpr(char op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
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
