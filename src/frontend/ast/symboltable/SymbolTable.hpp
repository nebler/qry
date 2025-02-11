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

// // Example usage:
// void analyzeProgram(const std::vector<std::unique_ptr<Stmt>> &ast) {
//   // Create our symbol table
//   SymbolTable symbolTable;

//   // Create our visitor
//   SymbolTableBuilder builder(symbolTable);

//   // Visit each statement in the program
//   for (const auto &stmt : ast) {
//     stmt->accept(builder);
//   }

//   // Now symbolTable contains information about all variables
// }