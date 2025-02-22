#pragma once

enum TokenType {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_float_number = -5,
  tok_var = -6,
  tok_int_number = -7,
  tok_bool_value = -8,
  tok_struct = -9,
  tok_comma = ',',
  tok_right_paren = ')',
  tok_left_paren = '(',
  tok_left_bracket = '{',
  tok_right_bracket = '}',
  tok_plus = '+',
  tok_minus = '-',
  tok_slash = '/',
  tok_caret = '^',
  tok_asterix = '*',
  tok_tilde = '~',
  tok_bang = '!',
  tok_colon = ':',
  tok_point = '.',
  tok_dobule_quotes = '"',
  tok_semicolon = ';',
  tok_assign = '=',
  tok_undefined = -999,
  tok_int_type = -100,
  tok_bool_type = -101,
  tok_float_type = -102,
  tok_string_type = -103
};
