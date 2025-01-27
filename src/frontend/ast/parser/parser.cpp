#include "Parser.hpp"
#include "../types/Types.hpp"
#include "./Precedence.hpp"
#include "./parslets/BinaryOperatorParslet.hpp"
#include "./parslets/IdentifierParslet.hpp"
#include "./parslets/PostfixOperatorParslet.hpp"
#include "./parslets/PrefixOperatorParslet.hpp"
#include <iostream>

#include <memory>

class qryParser : public Parser {
  qryParser(Lexer *lexer) : Parser(lexer) {
    // Register all of the parselets for the grammar.

    // Register the ones that need special parselets.
    registerParselet(tok_identifier, std::unique_ptr<IdentifierParslet>(
                                         new IdentifierParslet()));
    registerParselet(tok_right_paren,
                     std::unique_ptr<AssignParselet>(new AssignParselet()));
    registerParselet(tok_left_paren,
                     std::unique_ptr<GroupParselet>(new GroupParselet()));
    registerParselet(tok_left_paren,
                     std::unique_ptr<CallParselet>(new CallParselet()));

    // Register the simple operator parselets.
    prefix(tok_plus, precedence::PREFIX);
    prefix(tok_minus, precedence::PREFIX);
    prefix(tok_tilde, precedence::PREFIX);
    prefix(tok_bang, precedence::PREFIX);

    // For kicks, we'll make "!" both prefix and postfix, kind of like ++.
    postfix(tok_bang, precedence::POSTFIX);

    infixLeft(tok_plus, precedence::SUM);
    infixLeft(tok_minus, precedence::SUM);
    infixLeft(tok_asterix, precedence::PRODUCT);
    infixLeft(tok_slash, precedence::PRODUCT);
    infixRight(tok_caret, precedence::EXPONENT);
  }

  /**
   * Registers a postfix unary operator parselet for the given token and
   * precedence.
   */
  void postfix(Token token, int precedence) {
    registerParselet(token, std::unique_ptr<PostfixOperatorParselet>(
                                new PostfixOperatorParselet(precedence)));
  }

  /**
   * Registers a prefix unary operator parselet for the given token and
   * precedence.
   */
  void prefix(Token token, int precedence) {
    registerParselet(token, std::unique_ptr<PrefixOperatorParselet>(
                                new PrefixOperatorParselet(precedence)));
  }

  /**
   * Registers a left-associative binary operator parselet for the given token
   * and precedence.
   */
  void infixLeft(Token token, int precedence) {
    registerParselet(token, std::unique_ptr<BinaryOperatorParselet>(
                                new BinaryOperatorParselet(precedence, false)));
  }

  /**
   * Registers a right-associative binary operator parselet for the given token
   * and precedence.
   */
  void infixRight(Token token, int precedence) {
    registerParselet(token, std::unique_ptr<BinaryOperatorParselet>(
                                new BinaryOperatorParselet(precedence, true)));
  }
};