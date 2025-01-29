#pragma once

#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/expression/Expr.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
#include <map>

class InfixParselet;
class PrefixParselet;

class Parser {

private:
  Lexer *lexer;
  Token currentToken;

  std::map<Token, std::unique_ptr<PrefixParselet>> prefixParselets;
  std::map<Token, std::unique_ptr<InfixParselet>> infixParselets;

public:
  Parser(Lexer *lexer) : lexer(lexer) {};

  void registerParselet(Token token, std::unique_ptr<PrefixParselet> parselet) {
    prefixParselets[token] = std::move(parselet);
  }
  void registerParselet(Token token, std::unique_ptr<InfixParselet> parselet) {
    infixParselets[token] = std::move(parselet);
  }
};