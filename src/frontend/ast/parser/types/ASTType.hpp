#pragma once

enum ASTType { INT, FLOAT, STRING, BOOL, UNKNOWN };

// We can add this as a standalone function that works with the enum
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
  default:
    return "invalid";
  }
}