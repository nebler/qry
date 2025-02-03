#pragma once

#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/parselets/infix/BinaryOperatorParselet.hpp"
#include "frontend/ast/parser/parselets/infix/CallParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/GroupParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/IdentifierParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/NumberParselet.hpp"
#include "frontend/ast/parser/parselets/prefix/PrefixOperatorParselet.hpp"
class qryParser : public Parser {
public:
  qryParser(Lexer *lexer) : Parser(lexer) {

    registerParselet(tok_identifier, std::unique_ptr<IdentifierParselet>(
                                         new IdentifierParselet()));

    registerParselet(tok_left_paren,
                     std::unique_ptr<CallParselet>(new CallParselet()));

    registerParselet(tok_left_paren,
                     std::unique_ptr<GroupParselet>(new GroupParselet()));

    registerParselet(tok_assign,
                     std::unique_ptr<GroupParselet>(new GroupParselet()));
    registerParselet(tok_number,
                     std::unique_ptr<NumberParselet>(new NumberParselet()));
    // Register the simple operator parselets.
    prefix(tok_plus, PREFIX);
    prefix(tok_minus, PREFIX);
    prefix(tok_asterix, PREFIX);
    prefix(tok_slash, PREFIX);
    prefix(tok_caret, PREFIX);

    infixLeft(tok_plus, SUM);
    infixLeft(tok_minus, SUM);
    infixLeft(tok_asterix, PRODUCT);
    infixLeft(tok_slash, PRODUCT);
    infixRight(tok_caret, EXPONENT);
  };

private:
  void infixLeft(TokenType token, int precedence) {
    registerParselet(token, std::unique_ptr<BinaryOperatorParselet>(
                                new BinaryOperatorParselet(precedence, false)));
  }

  void infixRight(TokenType token, int precedence) {
    registerParselet(token, std::unique_ptr<BinaryOperatorParselet>(
                                new BinaryOperatorParselet(precedence, true)));
  }

  /**
   * Registers a prefix unary operator parselet for the given token and
   * precedence.
   */
  void prefix(TokenType token, int precedence) {
    registerParselet(token, std::unique_ptr<PrefixOperatorParselet>(
                                new PrefixOperatorParselet(precedence)));
  }
};