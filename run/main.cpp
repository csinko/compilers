#include "mc-compiler/file.hpp"
#include "mc-compiler/lexer.hpp"
#Include "mc-compiler/parser.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  file input(argv[1]);
  symbol_table syms;

  parser p(syms, input);
  p.parse_declaration();
}
