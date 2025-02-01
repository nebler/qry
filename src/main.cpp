#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include "frontend/ast/parser/qryParser.hpp"
#include <iostream>

#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Could not open file: " << argv[1] << std::endl;
    return 1;
  }
  Lexer lexer(file);
  qryParser parser(&lexer);
  while (true) {

    std::unique_ptr<Expr> result = parser.parseExpression();

    std::cout << "we parsed:" << result->print() << std::endl;
  }
  return 0;
}