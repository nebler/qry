#include "Parser.hpp"
#include <iostream>
#include <memory>
#include "../types/Types.hpp"

std::map<char, int> Parser::BinopPrecedence;

std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(lexer->getNumVal());
  getNextToken(); // consume the number
  return std::move(Result);
}

std::unique_ptr<ExprAST> Parser::LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> Parser::LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
  getNextToken(); // eat (.
  auto V = ParseExpression();
  if (!V)
    return nullptr;

  if (currentToken != ')')
    return LogError("expected ')'");
  getNextToken(); // eat ).
  return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
  std::string IdName = lexer->getIdentifierStr();

  getNextToken(); // eat identifier.

  if (currentToken != '(') {
      return std::make_unique<VariableExprAST>(IdName, TypeKind::IntegerType);
  }

  // Call.
  getNextToken(); // eat (
  std::vector<std::unique_ptr<ExprAST>> Args;
  if (currentToken != ')') {
    while (true) {
      if (auto Arg = ParseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr;

      if (currentToken == ')')
        break;

      if (currentToken != ',')
        return LogError("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  // Eat the ')'.
  getNextToken();

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> Parser::ParsePrimary() {
  switch (currentToken) {
  default:

    return LogError("unknown token when expecting an expression");
  case tok_identifier:

    return ParseIdentifierExpr();
  case tok_number:

    return ParseNumberExpr();
  case '(':
    return ParseParenExpr();
  }
}

/// binoprhs
///   ::= ('+' primary)*
std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec,
                                               std::unique_ptr<ExprAST> LHS) {
  // If this is a binop, find its precedence.
  while (true) {
    int TokPrec = getBinopPrecedence(currentToken);

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = currentToken;
    getNextToken(); // eat binop

    // Parse the primary expression after the binary operator.
    auto RHS = ParsePrimary();
    if (!RHS)
      return nullptr;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = getBinopPrecedence(currentToken);
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    // Merge LHS/RHS.
    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

/// expression
///   ::= primary binoprhs
///
std::unique_ptr<ExprAST> Parser::ParseExpression() {
  auto LHS = ParsePrimary();
  if (!LHS)
    return nullptr;
  return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {

  if (currentToken != tok_identifier) {

    return LogErrorP("Expected function name in prototype");
  }

  std::string FnName = lexer->getIdentifierStr();
  getNextToken();

  if (currentToken != '(') {

    return LogErrorP("Expected '(' in prototype");
  }

  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(lexer->getIdentifierStr());
  if (currentToken != ')') {

    return LogErrorP("Expected ')' in prototype");
  }

  // success.
  getNextToken(); // eat ')'.

  return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
  getNextToken(); // eat def.
  auto Proto = ParsePrototype();
  if (!Proto)
    return nullptr;

  if (auto E = ParseExpression()) {
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }

  return nullptr;
}

/// toplevelexpr ::= expression
std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
  if (auto E = ParseExpression()) {
    // Make an anonymous proto.
    auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                std::vector<std::string>());
    return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
  }
  return nullptr;
}

/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
  getNextToken(); // eat extern.
  return ParsePrototype();
}

void Parser::HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

void Parser::HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

void Parser::HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
void Parser::MainLoop() {
  fprintf(stderr, "ready>");
  getNextToken();
  while (true) {
    switch (currentToken) {
    case tok_eof:
      fprintf(stderr, "done");
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}
