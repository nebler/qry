#pragma once

#include <string>
enum ASTType { INT, FLOAT, STRING, BOOL, UNKNOWN, STRUCT, VOID, ARRAY };

inline std::string toString(ASTType type) {
  switch (type) {
  case ASTType::INT:
    return "int";
  case ASTType::FLOAT:
    return "float";
  case ASTType::STRING:
    return "string";
  case ASTType::BOOL:
    return "bool";
  case ASTType::UNKNOWN:
    return "unknown";
  case ASTType::STRUCT:
    return "struct";
  case ASTType::ARRAY:
    return "struct";
  default:
    return "invalid";
  }
}

class TypeReference {

public:
  ASTType basicType = ASTType::UNKNOWN;
  std::optional<std::string> customTypeName;

  TypeReference(ASTType type) : basicType(type) {}

  TypeReference(std::string name)
      : basicType(ASTType::STRUCT), customTypeName(std::move(name)) {}

  std::string toString() const {
    if (customTypeName) {
      return *customTypeName;
    }
    return ::toString(basicType);
  }
};