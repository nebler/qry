#pragma once

#include "frontend/ast/lexer/token/Token.hpp"
#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/types/ASTType.hpp"
inline ASTType tokenToTypeConverter(Token token) {
  switch (token.getType()) {
  case tok_int_type:
    return ASTType::INT;
  case tok_bool_type:
    return ASTType::BOOL;
  case tok_float_type:
    return ASTType::FLOAT;
  case tok_string_type:
    return ASTType::STRING;
  default:
    return ASTType::UNKNOWN;
  }
}
