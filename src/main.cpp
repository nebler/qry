#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"
#include <iostream>

#include <iostream>

int main() {

  std::string str = "a + b"; /* initial string */
  std::istringstream in(str);
  Lexer lexer = Lexer(in);
  qryParser parser = qryParser(&lexer);

  while (true) {

    std::unique_ptr<Expr> result = parser.parseExpression();

    std::cout << "we parsed:" << result->print() << std::endl;
  }
  return 0;
}