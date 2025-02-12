#pragma once

#include "frontend/ast/parser/ast/ASTNode.hpp"
#include <iostream>

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

  std::string symbolKindToString(SymbolKind kind) const {
    switch (kind) {
    case SymbolKind::Variable:
      return "Variable";
    case SymbolKind::Function:
      return "Function";
    case SymbolKind::Class:
      return "Class";
    case SymbolKind::Field:
      return "Field";
    case SymbolKind::Method:
      return "Method";
    case SymbolKind::Namespace:
      return "Namespace";
    case SymbolKind::Type:
      return "Type";
    case SymbolKind::Module:
      return "Module";
    default:
      return "Unknown";
    }
  }
  void print() const {
    // Print symbol header with name and kind
    std::cout << "Symbol: " << name << " (" << symbolKindToString(kind)
              << ")\n";

    // Print definition information
    std::cout << "Definition:\n";
    if (definition) {
      std::cout << "  ";                // Indent for readability
      std::cout << definition->print(); // Using ASTNode's print method
    } else {
      std::cout << "  <no definition>\n";
    }

    // Print all uses with indices for clarity
    std::cout << "Uses (" << uses.size() << " total):\n";
    for (size_t i = 0; i < uses.size(); i++) {
      std::cout << "  Use " << (i + 1) << ":\n";
      std::cout << "    "; // Double indent for use entries
      if (uses[i]) {
        std::cout << uses[i]->print();
      } else {
        std::cout << "<null use>\n";
      }
    }

    // Add a separator line for clarity when printing multiple symbols
    std::cout << "----------------------------------------\n";
  }
};