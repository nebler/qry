#pragma once

#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/expression/Expr.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
#include <unordered_map>

class InfixParselet;
class PrefixParselet;

class Parser {

private:
  Lexer *lexer;
  Token currentToken;

  std::unordered_map<Token, std::unique_ptr<PrefixParselet>> prefixParselets;
  std::unordered_map<Token, std::unique_ptr<InfixParselet>> infixParselets;

  void registerParselet(Token token, std::unique_ptr<PrefixParselet> parselet) {
    prefixParselets[token] = std::move(parselet);
  }
  void registerParselet(Token token, std::unique_ptr<InfixParselet> parselet) {
    infixParselets[token] = std::move(parselet);
  }

  int getPrecedence() {
    auto itParser = infixParselets.find(currentToken);
    if (itParser != infixParselets.end())
      return itParser->second->getPrecedence();
    return 0;
  }

public:
  Parser(Lexer *lexer) : lexer(lexer) {};

  std::unique_ptr<Expr> parseExpression(int precedence) {
    Token token = consume();
    auto it = prefixParselets.find(token);
    PrefixParselet *prefix = it->second.get();
    std::unique_ptr<Expr> left = prefix->parse(*this, token);
    while (precedence < getPrecedence()) {
      token = lexer->gettok();
      token = consume();
      InfixParselet *infix = infixParselets[token].get();
      left = infix->parse(*this, std::move(left), token);
    }
    return left;
  }

  std::unique_ptr<Expr> parseExpression() { return parseExpression(0); }
  bool match(Token expected) { return expected == currentToken; }

  Token consume(Token expected) {
    Token token = currentToken;
    // if (token != expected) {
    //   throw ParseException("Expected token " + tokentype::toString(expected)
    //   +
    //                        " and found " +
    //                        tokentype::toString(token.getType()));
    // }
    return consume();
  }

  Token consume() { return lexer->gettok(); }
};