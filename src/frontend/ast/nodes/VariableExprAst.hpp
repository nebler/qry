#pragma once
#include "../types/Types.hpp"
#include "ExprAST.hpp"
#include <string>

class VariableExprAST : public ExprAST {
  std ::string Name;
  TypeKind Type;

public:
  VariableExprAST(const std::string &Name, const TypeKind Type)
      : Name(Name), Type(Type) {}
};