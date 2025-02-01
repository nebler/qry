#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"
#include <iostream>

#include <iostream>

int main() {
  Lexer lexer = Lexer(std::cin);
  qryParser parser = qryParser(&lexer);

  while (true) {

    std::unique_ptr<Expr> result = parser.parseExpression();

    std::cout << result->print() << std::endl;
  }
  return 0;
}