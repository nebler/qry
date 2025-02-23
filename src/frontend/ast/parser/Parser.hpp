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
      consume();
    }
    return std::make_unique<ProgramNode>(std::move(statements));
  }

  std::unique_ptr<Stmt> parseStatement() {

    if (currentToken.getType() == tok_var) {

      return parseVarDeclaration();
    }

    if (currentToken.getType() == tok_return) {
      return parseReturnStatement();
    }

    if (currentToken.getType() == tok_struct) {
      return parseStructDeclaration();
    }

    if (currentToken.getType() == tok_fn) {
      return parseFunctionDeclaration();
    }

    return parseExpressionStatement();
  }

  std::unique_ptr<Stmt> parseFunctionDeclaration() {
    Token name = consume();

    consume(); // (

    std::map<std::string, TypeReference> parameters;

    Token token =
        consume(); // consume next one there might not be any arguments

    // todo: super ugly code pls refactor me
    while (token.getType() != tok_right_paren) {
      consume();                   // :
      Token typeToken = consume(); // type
      TypeReference type = TypeReference(tokenToTypeConverter(typeToken));
      parameters.insert({token.getText(), type});
      token = consume();
      if (token.getType() == tok_comma) {
        token = consume();
      }
    }
    consume(); // )

    TypeReference type = token.getType() != tok_left_bracket
                             ? TypeReference(tokenToTypeConverter(token))
                             : TypeReference(ASTType::VOID);

    if (token.getType() != tok_left_bracket) {
      consume(); // if type is present eat it
      std::cout << currentToken.getType() << currentToken.getText()
                << std::endl;
    }

    std::vector<std::unique_ptr<Stmt>> stmts;
    consume();
    while (currentToken.getType() != tok_right_bracket) {

      std::unique_ptr<Stmt> stmt = parseStatement();
      stmts.push_back(std::move(stmt));
      consume();
    }
    std::unique_ptr<BlockStmt> body =
        std::make_unique<BlockStmt>(std::move(stmts));

    // std::string name;
    // std::map<ASTType, std::string> parameter;
    // std::unique_ptr<BlockStmt> body;
    // TypeReference type;

    return std::make_unique<FunctionDeclarationStmt>(
        std::move(name.getText()), std::move(parameters), std::move(type),
        std::move(body));
  }

  std::unique_ptr<Stmt> parseReturnStatement() {
    std::unique_ptr<Expr> expression = parseExpression();
    consume();

    return std::make_unique<ReturnStmt>(std::move(expression));
  }

  std::unique_ptr<Stmt> parseStructDeclaration() {
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
    consume();
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
    //    throw ParseException("Expected token " +
    //    tokentype::toString(expected)
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