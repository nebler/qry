#pragma once

#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/expression/Expr.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
#include <iostream>
#include <unordered_map>

class InfixParselet;
class PrefixParselet;

class Parser {

private:
  Lexer *lexer;
  Token currentToken;

  std::unordered_map<TokenType, std::unique_ptr<PrefixParselet>>
      prefixParselets;
  std::unordered_map<TokenType, std::unique_ptr<InfixParselet>> infixParselets;

  int getPrecedence() {
    auto itParser = infixParselets.find(currentToken.getType());
    if (itParser != infixParselets.end())
      return itParser->second->getPrecedence();
    return 0;
  }

public:
  void registerParselet(TokenType token,
                        std::unique_ptr<PrefixParselet> parselet) {
    prefixParselets[token] = std::move(parselet);
  }
  void registerParselet(TokenType token,
                        std::unique_ptr<InfixParselet> parselet) {
    infixParselets[token] = std::move(parselet);
  }

  Parser(Lexer *lexer) : lexer(lexer) {};

  std::unique_ptr<Expr> parseExpression(int precedence) {
    Token token = consume();
    std::cout << token.getText() << std::endl;
    std::cout << token.getType() << std::endl;
    auto it = prefixParselets.find(token.getType());
    PrefixParselet *prefix = it->second.get();
    std::unique_ptr<Expr> left = prefix->parse(*this, token);

    while (precedence < getPrecedence()) {
      token = lexer->gettok();
      token = consume();
      InfixParselet *infix = infixParselets[token.getType()].get();
      left = infix->parse(*this, std::move(left), token);
    }
    return left;
  }

  std::unique_ptr<Expr> parseExpression() { return parseExpression(0); }
  bool match(TokenType expected) { return expected == currentToken.getType(); }

  Token consume(TokenType expected) {
    Token token = currentToken;

    // todo: fix this
    //  if (token != expected) {
    //    throw ParseException("Expected token " + tokentype::toString(expected)
    //    +
    //                         " and found " +
    //                         tokentype::toString(token.getType()));
    //  }
    return consume();
  }

  Token consume() { return lexer->gettok(); }
};