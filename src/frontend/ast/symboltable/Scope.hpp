#pragma once
#include "frontend/ast/symboltable/Symbol.hpp"
#include <map>
#include <memory>
enum class ScopeType { Global, Function, Block, Class, Namespace, Loop };
class Scope {
private:
  std::map<std::string, Symbol> symbols;
  Scope *parent;
  std::vector<std::unique_ptr<Scope>> children;
  ScopeType scopeType;

public:
  explicit Scope(Scope *parentScope = nullptr,
                 ScopeType scopeType = ScopeType::Global)
      : parent(parentScope), scopeType(scopeType) {}

  void declare(const std::string &name, const ASTNode *definition) {
    if (symbols.find(name) != symbols.end()) {
      throw std::runtime_error("Variable '" + name +
                               "' already declared in current scope");
    }
    symbols.emplace(name, Symbol(name, definition));
  }

  Scope *getParent() const { return parent; }

  void addChild(std::unique_ptr<Scope> scope) {
    children.push_back(std::move(scope));
  }

  Symbol *lookupLocal(const std::string &name) {
    auto it = symbols.find(name);
    return it != symbols.end() ? &it->second : nullptr;
  }

  const std::vector<std::unique_ptr<Scope>> &getChildren() const {
    return children;
  }
};