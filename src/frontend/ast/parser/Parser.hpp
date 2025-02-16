#pragma once

#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/parselets/infix/InfixParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixParselet.hpp"
#include "frontend/ast/parser/types/ASTType.hpp"
#include "frontend/ast/parser/types/TokenToConverter.hpp"
#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
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
    auto itParser = infixParselets.find(lookAhead().getType());
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
    auto it = prefixParselets.find(token.getType());
    PrefixParselet *prefix = it->second.get();

    std::unique_ptr<Expr> left = prefix->parse(*this, token);
    while (precedence < getPrecedence()) {
      token = consume();
      InfixParselet *infix = infixParselets[token.getType()].get();
      left = infix->parse(*this, std::move(left), token);
    }
    return left;
  }

  Token lookAhead() { return lexer->lookAhead(1); }

  std::unique_ptr<ProgramNode> parse() {

    std::vector<std::unique_ptr<Stmt>> statements;
    consume();

    while (currentToken.getType() != tok_eof) {
      statements.push_back(parseStatement());
      if (lookAhead().getType() == tok_eof) {
        break;
      }
    }
    return std::make_unique<ProgramNode>(std::move(statements));
  }

  std::unique_ptr<Stmt> parseStatement() {

    if (currentToken.getType() == tok_var) {
      return parseVarDeclaration();
    }

    if (currentToken.getType() == tok_struct) {
      return parseStructDeclaration();
    }

    return parseExpressionStatement();
  }

  std::unique_ptr<Stmt> parseStructDeclaration() {
    std::cout << "hello world" << std::endl;
    Token nameToken = consume(tok_identifier);
    std::string name = nameToken.getText();
    consume();

    std::map<std::string, TypeReference> fields;
    while (currentToken.getType() != tok_right_bracket) {
      Token token = consume(tok_identifier);
      std::string name = token.getText();

      consume();

      Token typeToken = consume();

      TypeReference type = TypeReference(tokenToTypeConverter(currentToken));

      if (type.basicType == ASTType::UNKNOWN) {
        fields.insert({name, TypeReference(currentToken.getText())});
      }
      fields.insert({name, TypeReference(type)});

      consume();
    }
    return std::make_unique<StructDeclarationStmt>(std::move(name),
                                                   std::move(fields));
  }

  std::unique_ptr<Stmt> parseVarDeclaration() {
    Token nameToken = consume(tok_identifier);
    std::string name = nameToken.getText();

    consume(tok_colon);

    auto type = TypeReference(tokenToTypeConverter(currentToken));

    consume(tok_assign);

    auto initializer = parseExpression();
    consume(tok_semicolon);
    return std::make_unique<VarDeclarationStmt>(std::move(name),
                                                std::move(initializer), type);
  }

  std::unique_ptr<Stmt> parseExpressionStatement() {
    auto expr = parseExpression();
    consume(tok_semicolon);
    return std::make_unique<ExpressionStmt>(std::move(expr));
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

  Token consumeText() {
    currentToken = lexer->gettok(false);
    return currentToken;
  }

  Token consume() {
    currentToken = lexer->gettok();
    return currentToken;
  }
};