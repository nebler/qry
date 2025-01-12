#pragma once

enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_number = -5,

  tok_int = -100,
  tok_bool = -101,
  tok_float = -102
};
