
#include "PrefixParselet.hpp"
/**
 * Parses parentheses used to group an expression, like "a * (b + c)".
 */
class GroupParselet : public PrefixParselet {
  virtual std::unique_ptr<Expr> parse(Parser &parser,
                                      int token) const override {
    std::unique_ptr<Expr> expression = parser.parseExpression();
    parser.consume(tok_right_paren);
    return expression;
  }
};
