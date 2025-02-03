#pragma once

enum TokenType {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_number = -5,
  tok_var = -6,
  tok_comma = ',',
  tok_right_paren = ')',
  tok_left_paren = '(',
  tok_plus = '+',
  tok_minus = '-',
  tok_slash = '/',
  tok_caret = '^',
  tok_asterix = '*',
  tok_tilde = '~',
  tok_bang = '!',
  tok_semicolon = ';',
  tok_assign = '=',
  tok_undefined = -999,
  tok_int = -100,
  tok_bool = -101,
  tok_float = -102
};
