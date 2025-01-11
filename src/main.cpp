#include "frontend/ast/lexer/Lexer.hpp"
#include <iostream>
#include <sstream>

int main() {
  std::istringstream input("def foo(x) 42.0");
  Lexer lexer = Lexer(input);
  lexer.gettok();
  lexer.gettok();
  lexer.gettok();
  lexer.gettok();
  lexer.gettok();
  lexer.gettok();
  lexer.gettok();

  return 0;
}