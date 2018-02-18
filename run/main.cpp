#include "mc-compiler/file.hpp"
#include "mc-compiler/lexer.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  file input(argv[1]);
  symbol_table syms;
  lexer lex(syms, input);
  while (token tok = lex())
    std::cout << tok << '\n';
}
