#include "Lexer.hpp"
#include <cctype>
#include <iostream>

int Lexer::advance() { return input->get(); }

/*
  Todo: HUGE hack.
  Maybe try and make the gettok method more stateless?
*/
Token Lexer::lookAhead(int distance) {
  int pos = input->tellg();
  auto charBefore = lastChar;
  auto numbefore = numVal;

  auto identStrBefore = this->identifierStr;
  std::vector<Token> tokens;

  for (int i = 0; i < distance; ++i) {
    tokens.push_back(gettok());
  }

  if (tokens.empty()) {
    throw std::out_of_range("No tokens available");
  }

  input->seekg(pos);

  lastChar = charBefore;
  numVal = numbefore;
  identifierStr = identStrBefore;
  return tokens.back();
}

Token Lexer::findKeyWordToken(const char *key) {
  auto it = std::find_if(std::begin(KEYWORD_TOKENS), std::end(KEYWORD_TOKENS),
                         [key](const std::pair<const char *, TokenType> &p) {
                           return std::strcmp(p.first, key) == 0;
                         });
  if (it != std::end(KEYWORD_TOKENS)) {
    return Token{it->second, key};
  } else {
    return Token{tok_undefined, ""};
  }
}

Token Lexer::findCharToken(char key) {
  auto it = std::find_if(
      std::begin(CHAR_TOKENS), std::end(CHAR_TOKENS),
      [key](const std::pair<char, TokenType> &p) { return p.first == key; });
  if (it != std::end(CHAR_TOKENS)) {
    return Token{it->second, std::string(1, key)};
  } else {
    return Token{tok_undefined, ""};
  }
}

Token Lexer::gettok() {
  while (isspace(lastChar)) {
    lastChar = advance();
  }
  if (isalpha(lastChar)) {
    identifierStr = lastChar;
    while (isalnum((lastChar = advance()))) {
      identifierStr += lastChar;
    }
    Token token = findKeyWordToken(identifierStr.c_str());

    if (token.getType() == tok_undefined) {
      return Token{tok_identifier, identifierStr};
    }
    return token;
  }

  if (isdigit(lastChar) || lastChar == '.') {
    std::string numStr;
    bool isFloat = false;

    do {
      if (lastChar == '.') {
        isFloat = true;
      }
      numStr += lastChar;
      lastChar = advance();
    } while (isdigit(lastChar) || lastChar == '.');
    setNumVal(strtod(numStr.c_str(), nullptr));
    // Return the appropriate token based on whether we saw a decimal point
    return Token{isFloat ? tok_float_number : tok_int_number, numStr};
  }

  // simple look up into chars map
  Token token = findCharToken(lastChar);

  if (token.getType() != tok_undefined) {
    lastChar = advance();
    return token;
  }

  // Check for end of file
  if (lastChar == EOF) {
    return Token{tok_eof, ""};
  }

  return Token{tok_undefined, identifierStr};
}
