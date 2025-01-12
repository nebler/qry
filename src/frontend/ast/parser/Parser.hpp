#pragma once

#include "../lexer/Lexer.hpp"
#include "../nodes/BinaryExprAST.hpp"
#include "../nodes/CallExprAST.hpp"
#include "../nodes/ExprAST.hpp"
#include "../nodes/FunctionAST.hpp"
#include "../nodes/NumberExprAst.hpp"
#include "../nodes/PrototypeAST.hpp"
#include "../nodes/VariableExprAst.hpp"
#include <iostream>
#include <map>
#include <memory>

class Parser {
private:
  Lexer *lexer;
  int currentToken; // Stores the current token we're looking at
  static std::map<char, int> BinopPrecedence;
  // Helper methods for parsing different constructs
  std::unique_ptr<ExprAST> ParseNumberExpr();
  std::unique_ptr<ExprAST> ParseParenExpr();
  std::unique_ptr<ExprAST> ParseIdentifierExpr();
  std::unique_ptr<ExprAST> ParsePrimary();
  std::unique_ptr<ExprAST> ParseExpression();
  std::unique_ptr<PrototypeAST> ParsePrototype();
  std::unique_ptr<FunctionAST> ParseDefinition();
  std::unique_ptr<PrototypeAST> ParseExtern();
  std::unique_ptr<FunctionAST> ParseTopLevelExpr();
  std::unique_ptr<ExprAST> LogError(const char *Str);
  std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
  void HandleDefinition();
  void HandleExtern();
  void HandleTopLevelExpression();
  std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                         std::unique_ptr<ExprAST> LHS);

public:
  Parser(Lexer *lex) : lexer(lex) {
    // Prime the first token
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40; // highest.
  }

  void MainLoop();
  // Get the next token from the lexer
  int getNextToken() {
    currentToken = lexer->gettok();
    return currentToken;
  }

  // Get the next token from the lexer
  int getBinopPrecedence(char operatorName) {
    int TokPrec = BinopPrecedence[operatorName];
    if (TokPrec <= 0) {
      return -1;
    }
    return TokPrec;
  }
};