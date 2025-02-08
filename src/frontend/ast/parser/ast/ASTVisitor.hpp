#pragma once

// Make a visitor interface
#include "frontend/ast/parser/ast/ASTNode.hpp"
class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;

  // Expressions
  virtual void visitNumberExpr(NumberExpr *expr) = 0;
  virtual void visitIdentifierExpr(IdentifierExpr *expr) = 0;
  virtual void visitBinaryExpr(BinaryExpr *expr) = 0;
  virtual void visitPrefixExpr(PrefixExpr *expr) = 0;
  virtual void visitCallExpr(CallExpr *expr) = 0;
  virtual void visitAssignExpr(AssignExpr *expr) = 0;
};