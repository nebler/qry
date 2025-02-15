#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/ast/ASTNode.hpp"
#include "frontend/ast/parser/qryParser.hpp"
#include "frontend/ast/symboltable/SymbolTableBuilder.hpp"

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

  std::unique_ptr<ProgramNode> tree = parser.parse();

  std::cout << tree->print() << std::endl;

  std::unique_ptr<Scope> symbolTable =
      SymbolTableBuilder::createSymbolTable(tree);

  for (const auto &foo : symbolTable->symbols) {
    foo.second.print();
  }

  // type chekcing here?

  return 0;
}