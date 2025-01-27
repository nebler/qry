#pragma once

#include "../lexer/Lexer.hpp"
#include "../nodes/Expr.hpp"

#include "parslets/InfixParslet.hpp"
#include "parslets/PrefixParselet.hpp"
#include <map>
#include <memory>

class InfixParselet;


class Parser {
private:
  Lexer *lexer;
  Token currentToken; // Stores the current token we're looking at

  std::unique_ptr<Expr> ParseBinOpRHS(int ExprPrec, std::unique_ptr<Expr> LHS);

public:
  Parser(Lexer *lex) : lexer(lex) {};

  void registerParselet(Token token, std::unique_ptr<PrefixParselet> parselet) {
    mPrefixParselets[token] = std::move(parselet);
  }

  void registerParselet(Token token, std::unique_ptr<InfixParselet> parselet) {
    mInfixParselets[token] = std::move(parselet);
  }

  std::string getIndentifierStr() { return lexer->getIdentifierStr(); }

  // Get the next token from the lexer
  Token getNextToken() {
    currentToken = lexer->gettok();
    return currentToken;
  }

  std::unique_ptr<Expr> parseExpression(int precedence) {
    Token token = consume();
    auto it = mPrefixParselets.find(token);

    PrefixParselet *prefix = it->second.get();

    std::unique_ptr<Expr> left = prefix->parse(*this, token);

    while (precedence < getPrecedence()) {
      token = lexer->gettok();
      token = consume();

      InfixParselet *infix = mInfixParselets[token].get();
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

private:
  std::map<Token, std::unique_ptr<PrefixParselet>> mPrefixParselets;
  std::map<Token, std::unique_ptr<InfixParselet>> mInfixParselets;

  Token lookAhead(unsigned int distance) { return lexer->lookAhead(distance); }

  int getPrecedence() {
    auto itParser = mInfixParselets.find(currentToken);
    if (itParser != mInfixParselets.end())
      return itParser->second->getPrecedence();

    return 0;
  }
};
