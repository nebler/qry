#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/parser/Parser.hpp"
#include <iostream>
#include <sstream>

#include <iostream>

int main() {
  Lexer lexer = Lexer(std::cin);
  Parser parser = Parser(&lexer);
  parser.MainLoop();
  return 0;
}