#include "frontend/ast/lexer/Lexer.hpp"
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

  std::unique_ptr<ASTNode> tree = parser.parse();

  std::cout << tree->print() << std::endl;

  return 0;
}