#include "frontend/ast/lexer/Lexer.hpp"
#include "frontend/ast/lexer/token/Token.hpp"
#include "frontend/ast/lexer/token/TokenType.hpp"

#include <gtest/gtest.h>

TEST(Lexer, defToken) {

  std::istringstream input("def");

  Lexer lexer = Lexer(input);
  Token tokDefOutput = lexer.gettok();
  Token toDef = Token{tok_def, "def"};
  EXPECT_EQ(toDef, tokDefOutput);
}

TEST(Lexer, identifierToken) {

  std::istringstream input("foo");
  Lexer lexer = Lexer(input);
  Token identifier = lexer.gettok();
  Token tokIdentifier = Token{tok_identifier, "foo"};
  EXPECT_EQ(identifier, tokIdentifier);
}

TEST(Lexer, numberToken) {

  std::istringstream input("42.0");

  Lexer lexer = Lexer(input);
  Token token = lexer.gettok();
  Token tokNumberOutput = Token{tok_number, "42.0"};

  EXPECT_EQ(token, tokNumberOutput);
  EXPECT_EQ(lexer.getNumVal(), 42);
}

TEST(Lexer, externToken) {

  std::istringstream input("extern");

  Lexer lexer = Lexer(input);
  Token tokenExternOutput = lexer.gettok();
  Token tokExtern = Token{tok_extern, "extern"};

  EXPECT_EQ(tokExtern, tokenExternOutput);
}

TEST(Lexer, eofToken) {

  std::istringstream input("");

  Lexer lexer = Lexer(input);
  Token tokenEOFOutput = lexer.gettok();
  Token tokEOF = Token{tok_eof, ""};

  EXPECT_EQ(tokEOF, tokenEOFOutput);
}

TEST(Lexer, skipAllWhiteSpaces) {

  std::istringstream input("      def       extern      ");

  Lexer lexer = Lexer(input);
  Token tokDefOutput = lexer.gettok();

  Token tokDef = Token{tok_def, "def"};

  EXPECT_EQ(tokDef, tokDefOutput);

  Token tokenExternOutput = lexer.gettok();
  Token tokExtern = Token{tok_extern, "extern"};

  EXPECT_EQ(tokExtern, tokenExternOutput);

  Token tokenEOFOutput = lexer.gettok();
  Token tokEOF = Token{tok_eof, ""};

  EXPECT_EQ(tokEOF, tokenEOFOutput);
}

TEST(Lexer, allTokens) {
  std::istringstream input("def foo(x) extern 42.0");

  Lexer lexer = Lexer(input);

  // Expect token: "def"
  Token tokDefOutput = lexer.gettok();
  Token tokDef = Token{tok_def, "def"};
  EXPECT_EQ(tokDef, tokDefOutput);

  // Expect token: "foo"
  Token identifier = lexer.gettok();
  Token tokIdentifier = Token{tok_identifier, "foo"};
  EXPECT_EQ(identifier, tokIdentifier);

  // Expect token: "("
  Token leftParenOutput = lexer.gettok();
  Token leftParen = Token{tok_left_paren, "("};
  EXPECT_EQ(leftParenOutput, leftParen);

  // Expect token: "x"
  Token tokIdentifierX = lexer.gettok();
  Token tokX = Token{tok_identifier, "x"};
  EXPECT_EQ(tokIdentifierX, tokX);

  // Expect token: ")"
  Token rightParenOutput = lexer.gettok();
  Token rightParen = Token{tok_right_paren, ")"};
  EXPECT_EQ(rightParenOutput, rightParen);

  // Expect token: "extern"
  Token tokenExternOutput = lexer.gettok();
  Token tokExtern = Token{tok_extern, "extern"};
  EXPECT_EQ(tokExtern, tokenExternOutput);

  // Expect token: "42.0"
  Token tokenNumberOutput = lexer.gettok();
  Token tokNumber = Token{tok_number, "42.0"};
  EXPECT_EQ(tokNumber, tokenNumberOutput);
  EXPECT_EQ(lexer.getNumVal(), 42.0); // Verify numeric value

  // Expect token: eof
  Token tokEOFOut = lexer.gettok();
  Token tokEOF = Token{tok_eof, ""};
  EXPECT_EQ(tokEOF, tokEOFOut);
}

TEST(Lexer, moreComplexFunction) {
  std::istringstream input("def foo(x y) x+foo(y, 4.0);");

  Lexer lexer = Lexer(input);

  // Expect token: "def"
  Token tokDefOutput = lexer.gettok();
  Token tokDef = Token{tok_def, "def"};
  EXPECT_EQ(tokDef, tokDefOutput);

  // Expect token: "foo"
  Token tokIndentifierOutputFoo = lexer.gettok();
  Token tokFoo = Token{tok_identifier, "foo"};
  EXPECT_EQ(tokIndentifierOutputFoo, tokFoo);

  // Expect token: "("
  Token tokLeftParen = lexer.gettok();
  std::cout << tokLeftParen.getText() << std::endl;
  Token tokLeftParenExpected = Token{tok_left_paren, "("};
  EXPECT_EQ(tokLeftParen, tokLeftParenExpected);

  // Expect token: "x"
  Token tokX = Token{tok_identifier, "x"};
  Token tokIdentifierX = lexer.gettok();
  std::cout << tokIdentifierX.getText() << std::endl;
  std::cout << tokIdentifierX.getType() << std::endl;
  EXPECT_EQ(tokIdentifierX, tokX);

  // Expect token: "y"
  Token tokIdentifierY = lexer.gettok();
  Token tokY = Token{tok_identifier, "y"};
  EXPECT_EQ(tokIdentifierY, tokY);

  // Expect token: ")"
  Token tokRightParen = lexer.gettok();
  Token tokRightParenExpected = Token{tok_right_paren, ")"};
  EXPECT_EQ(tokRightParen, tokRightParenExpected);

  // Expect token: "x"
  Token tokIdentifierX2 = lexer.gettok();
  EXPECT_EQ(tokX, tokIdentifierX2);

  // Expect token: "+"
  Token plus = lexer.gettok();
  Token tokPlus = Token{tok_plus, "+"};
  EXPECT_EQ(plus, tokPlus);

  // Expect token: "foo"
  Token tokFoo2 = lexer.gettok();
  Token tokFooExpected = Token{tok_identifier, "foo"};
  EXPECT_EQ(tokFoo2, tokFooExpected);

  // Expect token: "("
  Token tokLeftParen2 = lexer.gettok();
  Token tokLeftParenExpected2 = Token{tok_left_paren, "("};
  EXPECT_EQ(tokLeftParen2, tokLeftParenExpected2);

  // Expect token: "y"
  Token tokIdentifierY2 = lexer.gettok();
  Token tokYExpected = Token{tok_identifier, "y"};
  EXPECT_EQ(tokIdentifierY2, tokYExpected);

  // Expect token: ","
  Token tokComma = lexer.gettok();
  Token tokCommaExpected = Token{tok_comma, ","};
  EXPECT_EQ(tokComma, tokCommaExpected);

  // Expect token: "4.0"
  Token tokenNumberOutput = lexer.gettok();
  Token tokNumber = Token{tok_number, "4.0"};
  EXPECT_EQ(tokNumber, tokenNumberOutput);
  EXPECT_EQ(lexer.getNumVal(), 4.0);

  // Expect token: ")"
  Token tokRightParen2 = lexer.gettok();
  Token tokRightParenExpected2 = Token{tok_right_paren, ")"};
  EXPECT_EQ(tokRightParen2, tokRightParenExpected2);

  // Expect token: ";"
  Token tokSemicolon = lexer.gettok();
  Token tokSemicolonExpected = Token{tok_semicolon, ";"};
  EXPECT_EQ(tokSemicolon, tokSemicolonExpected);

  // Expect token: eof
  Token tokEOFOut = lexer.gettok();
  Token tokEOF = Token{tok_eof, ""};
  EXPECT_EQ(tokEOF, tokEOFOut);
}
