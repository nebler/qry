#include "Parser.hpp"
#include <iostream>
#include <memory>
#include "../types/Types.hpp"

std::map<char, int> Parser::BinopPrecedence;
