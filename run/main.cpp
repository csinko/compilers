#include "mc-compiler/file.hpp"
#include "mc-compiler/lexer.hpp"
#include "mc-compiler/parser.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  file input(argv[1]);
  symbol_table syms;

  parser p(syms, input);
  decl* dec = p.parseDeclaration();
}
