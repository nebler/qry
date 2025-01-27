// Lexer.hpp
#pragma once
#include "Token.hpp"
#include <istream>
#include <string>
class Lexer {
private:
  std::istream *input;       // Input stream instead of using getchar()
  int lastChar;              // Track last character read
  std::string identifierStr; // Current identifier
  double numVal;             // Current number value
  // Helper method to get next character
  int advance();
  int peek();

public:
  // Constructor takes an input stream
  explicit Lexer(std::istream &in) : input(&in), lastChar(' ') {}

  // Token getters
  std::string getIdentifierStr() const { return identifierStr; }
  double getNumVal() const { return numVal; }

  void setIdentifierStr(const std::string &str) { identifierStr = str; }

  void setNumVal(double val) { numVal = val; }
  Token lookAhead(int distance);
  // Get next token
  Token gettok();
};
