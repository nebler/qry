#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"
#include <map>

class ASTVisitor;
struct Symbol {
  std::string name;
  const ASTNode *definition;
  bool isDefined;
  std::vector<const ASTNode *> uses;

  Symbol(std::string name, const ASTNode *def)
      : name(std::move(name)), definition(def), isDefined(false) {}
};

class SymbolTable {
private:
  std::map<std::string, Symbol> symbols;

public:
  void declare(const std::string &name, const ASTNode *definition) {
    if (symbols.find(name) != symbols.end()) {

      // todo: dont crash rather tell the user the error with the line and all
      throw std::runtime_error("Variable '" + name + "' already declared");
    }
    symbols.emplace(name, Symbol(name, definition));
  }

  void define(const std::string &name) {
    auto it = symbols.find(name);
    if (it == symbols.end()) {
      throw std::runtime_error("Variable '" + name + "' not declared");
    }
    it->second.isDefined = true;
  }

  // Record a usage of a symbol
  void addUse(const std::string &name, const ASTNode *use) {
    auto it = symbols.find(name);
    if (it == symbols.end()) {
      throw std::runtime_error("Use of undeclared variable '" + name + "'");
    }
    it->second.uses.push_back(use);
  }

  // Check if a symbol exists
  bool isDeclared(const std::string &name) const {
    return symbols.find(name) != symbols.end();
  }

  // Check if a symbol has been defined
  bool isDefined(const std::string &name) const {
    auto it = symbols.find(name);
    return it != symbols.end() && it->second.isDefined;
  }

  // Get where a symbol was defined
  const ASTNode *getDefinition(const std::string &name) const {
    auto it = symbols.find(name);
    if (it == symbols.end()) {
      throw std::runtime_error("Variable '" + name + "' not found");
    }
    return it->second.definition;
  }
};

// Now let's create our visitor that builds the symbol table
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

// Example usage:
void analyzeProgram(const std::vector<std::unique_ptr<Stmt>> &ast) {
  // Create our symbol table
  SymbolTable symbolTable;

  // Create our visitor
  SymbolTableBuilder builder(symbolTable);

  // Visit each statement in the program
  for (const auto &stmt : ast) {
    stmt->accept(builder);
  }

  // Now symbolTable contains information about all variables
}