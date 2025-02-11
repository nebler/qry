#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"

enum class SymbolKind {
  Variable,
  Function,
  Class,
  Field,
  Method,
  Namespace,
  Type,
  Module
};

class Symbol {
private:
  std::string name;
  SymbolKind kind;
  const ASTNode *definition;
  std::vector<const ASTNode *> uses;

public:
  Symbol(std::string name, const ASTNode *definition,
         SymbolKind kind = SymbolKind::Variable)
      : name(std::move(name)), kind(kind), definition(definition) {}

  void addUse(const ASTNode *use) { uses.push_back(use); }

  // Add getters as needed
  const std::string &getName() const { return name; }
  SymbolKind getKind() const { return kind; }
  const ASTNode *getDefinition() const { return definition; }
  const std::vector<const ASTNode *> &getUses() const { return uses; }
};