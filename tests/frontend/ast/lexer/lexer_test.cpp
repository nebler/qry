#include "frontend/ast/lexer/Token.hpp"
#include "frontend/ast/lexer/Lexer.hpp"
#include <gtest/gtest.h>

TEST(Lexer, defToken) {

  std::istringstream input("def");

  Lexer lexer = Lexer(input);
  int tokDefOutput = lexer.gettok();
  EXPECT_EQ(tok_def, tokDefOutput);
}

TEST(Lexer, identifierToken) {

  std::istringstream input("foo");
  Lexer lexer = Lexer(input);
  int tokIndentifierOutputFoo = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputFoo);
  EXPECT_EQ("foo", lexer.getIdentifierStr());
}

TEST(Lexer, numberToken) {

  std::istringstream input("42.0");

  Lexer lexer = Lexer(input);
  int tokNumberOutput = lexer.gettok();
  EXPECT_EQ(tok_number, tokNumberOutput);
  EXPECT_EQ(lexer.getNumVal(), 42);
}

TEST(Lexer, externToken) {

  std::istringstream input("extern");

  Lexer lexer = Lexer(input);
  int tokenExternOutput = lexer.gettok();
  EXPECT_EQ(tok_extern, tokenExternOutput);
}

TEST(Lexer, eofToken) {

  std::istringstream input("");

  Lexer lexer = Lexer(input);
  int tokenEOFOutput = lexer.gettok();
  EXPECT_EQ(tok_eof, tokenEOFOutput);
}

TEST(Lexer, skipAllWhiteSpaces) {

  std::istringstream input("      def       extern      ");

  Lexer lexer = Lexer(input);
  int tokDefOutput = lexer.gettok();
  EXPECT_EQ(tok_def, tokDefOutput);

  int tokenExternOutput = lexer.gettok();
  EXPECT_EQ(tok_extern, tokenExternOutput);

  int tokenEOFOutput = lexer.gettok();
  EXPECT_EQ(tok_eof, tokenEOFOutput);
}

TEST(Lexer, allTokens) {

  std::istringstream input("def foo(x) extern 42.0");

  Lexer lexer = Lexer(input);
  int tokDefOutput = lexer.gettok();
  EXPECT_EQ(tok_def, tokDefOutput);

  int tokIndentifierOutputFoo = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputFoo);
  EXPECT_EQ("foo", lexer.getIdentifierStr());

  int tokIdentifierBracesOpen = lexer.gettok();
  EXPECT_EQ('(', tokIdentifierBracesOpen);
  int tokIndentifierOutputX = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputX);
  EXPECT_EQ("x", lexer.getIdentifierStr());
  int tokIdentifierBracesClose = lexer.gettok();
  EXPECT_EQ(')', tokIdentifierBracesClose);

  int tokenExternOutput = lexer.gettok();
  EXPECT_EQ(tok_extern, tokenExternOutput);

  int tokenNumberOutput = lexer.gettok();
  EXPECT_EQ(tok_number, tokenNumberOutput);
  EXPECT_EQ(42, lexer.getNumVal());

  int tokEOFOut = lexer.gettok();
  EXPECT_EQ(tok_eof, tokEOFOut);
}

TEST(Lexer, moreComplexFunction) {

  std::istringstream input("def foo(x y) x+foo(y, 4.0);");

  Lexer lexer = Lexer(input);
  int tokDefOutput = lexer.gettok();
  EXPECT_EQ(tok_def, tokDefOutput);

  int tokIndentifierOutputFoo = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputFoo);
  EXPECT_EQ("foo", lexer.getIdentifierStr());

  int tokIdentifierBracesOpen = lexer.gettok();
  EXPECT_EQ('(', tokIdentifierBracesOpen);

  int tokIndentifierOutputX = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputX);
  EXPECT_EQ("x", lexer.getIdentifierStr());

  int tokIndentifierOutputY = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputY);
  EXPECT_EQ("y", lexer.getIdentifierStr());

  int tokIdentifierBracesClose = lexer.gettok();
  EXPECT_EQ(')', tokIdentifierBracesClose);

  int tokIndentifierOutputX2 = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputX2);
  EXPECT_EQ("x", lexer.getIdentifierStr());

  int plus = lexer.gettok();
  EXPECT_EQ('+', plus);

  int tokIndentifierOutputFoo2 = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputFoo2);
  EXPECT_EQ("foo", lexer.getIdentifierStr());

  int tokIdentifierBracesOpen2 = lexer.gettok();
  EXPECT_EQ('(', tokIdentifierBracesOpen2);

  int tokIndentifierOutputY2 = lexer.gettok();
  EXPECT_EQ(tok_identifier, tokIndentifierOutputY2);
  EXPECT_EQ("y", lexer.getIdentifierStr());

  int tokIdentifierComma = lexer.gettok();
  EXPECT_EQ(',', tokIdentifierComma);

  int tokenNumberOutput = lexer.gettok();
  EXPECT_EQ(tok_number, tokenNumberOutput);
  EXPECT_EQ(4, lexer.getNumVal());

  int tokIdentifierBracesClose2 = lexer.gettok();
  EXPECT_EQ(')', tokIdentifierBracesClose2);

  int tokIdentifierSemicolon = lexer.gettok();
  EXPECT_EQ(';', tokIdentifierSemicolon);

  int tokEOFOut = lexer.gettok();
  EXPECT_EQ(tok_eof, tokEOFOut);
}
