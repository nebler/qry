#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"
#include <iostream>

#include <iostream>

int main() {
  Lexer lexer = Lexer(std::cin);
  qryParser parser = qryParser(&lexer);

  std::cout << "laslal" << std::endl;
  std::unique_ptr<Expr> result = parser.parseExpression();
  return 0;
}