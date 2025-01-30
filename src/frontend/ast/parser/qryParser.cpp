

#include "frontend/ast/lexer/token/TokenType.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/Precedence.hpp"
#include "frontend/ast/parser/parselets/infix/BinaryOperatorParselet.hpp"
class qryParser : public Parser {

  qryParser(Lexer *lexer) : Parser(lexer) {
    infixLeft(tok_plus, SUM);
    infixLeft(tok_minus, SUM);
    infixLeft(tok_asterix, PRODUCT);
    infixLeft(tok_slash, PRODUCT);
    infixRight(tok_caret, EXPONENT);
  };

private:
  /**
   * Registers a left-associative binary operator parselet for the given token
   * and precedence.
   */
  void infixLeft(TokenType token, int precedence) {
    registerParselet(token, std::unique_ptr<BinaryOperatorParselet>(
                                new BinaryOperatorParselet(precedence, false)));
  }

  /**
   * Registers a right-associative binary operator parselet for the given token
   * and precedence.
   */
  void infixRight(TokenType token, int precedence) {
    registerParselet(token, std::unique_ptr<BinaryOperatorParselet>(
                                new BinaryOperatorParselet(precedence, true)));
  }
};