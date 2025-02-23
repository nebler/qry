#pragma once
#include "frontend/ast/lexer/token/Token.hpp"
#include "frontend/ast/lexer/token/TokenType.hpp"
#include <istream>
#include <string>

class Lexer {
private:
  int lastChar;
  std::istream *input;
  std::string identifierStr;
  double numVal;

  int advance();
  int peek();

  // This is evaluated at compile time
  static constexpr std::pair<const char *, TokenType> KEYWORD_TOKENS[] = {
      {"def", tok_def},         {"extern", tok_extern},
      {"int", tok_int_type},    {"bool", tok_bool_type},
      {"var", tok_var},         {"float", tok_float_type},
      {"str", tok_string_type}, {"true", tok_bool_value},
      {"struct", tok_struct},   {"false", tok_bool_value},
      {"fn", tok_fn},           {"return", tok_return}};
  static constexpr std::pair<char, TokenType> CHAR_TOKENS[] = {
      {'+', tok_plus},          {'-', tok_minus},       {',', tok_comma},
      {'*', tok_asterix},       {'/', tok_slash},       {'^', tok_caret},
      {'~', tok_tilde},         {'!', tok_bang},        {';', tok_semicolon},
      {'(', tok_left_paren},    {')', tok_right_paren}, {'=', tok_assign},
      {'"', tok_dobule_quotes}, {':', tok_colon},       {'{', tok_left_bracket},
      {'}', tok_right_bracket}, {'.', tok_point}};
  Token findKeyWordToken(const char *key);
  Token findCharToken(char key);

public:
  explicit Lexer(std::istream &in) : input(&in), lastChar(' ') {}

  std::string getIdentifierStr() const { return identifierStr; }
  double getNumVal() const { return numVal; }

  void setIdentifierStr(const std::string &str) { identifierStr = str; }

  void setNumVal(double val) { numVal = val; }
  Token lookAhead(int distance);
  Token gettok(bool skipAllWhiteSpaces = true);
};
