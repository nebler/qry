// Lexer.hpp
#pragma once
#include "frontend/ast/lexer/token/Token.hpp"
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

  // This is evaluated at compile time
  static constexpr std::pair<const char *, TokenType> KEYWORD_TOKENS[] = {
      {"def", tok_def},
      {"extern", tok_extern},
      {"int", tok_int},
      {"bool", tok_bool},
      {"float", tok_float}};
  static constexpr std::pair<char, TokenType> CHAR_TOKENS[] = {
      {'+', tok_plus},       {'-', tok_minus},      {',', tok_comma},
      {'*', tok_asterix},    {'/', tok_slash},      {'^', tok_caret},
      {'~', tok_tilde},      {'!', tok_bang},       {';', tok_semicolon},
      {'(', tok_left_paren}, {')', tok_right_paren}};
  Token findKeyWordToken(const char *key);
  Token findCharToken(char key);

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
