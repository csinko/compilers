#pragma once

#include "token.hpp"
#include <unordered_map>

class file;

class lexer {
  public:
    lexer(symbol_table& syms, const file& f);

    token operator()() {
      return scan(); 
    }

    token scan();

    bool eof() const;

    char peek() const;
    char peek(int n) const;


  private:
    char accept();
    void accept(int n);
    char ignore();

    void skipSpace();
    void skipNewline();
    void skipComment();

    token lexPunc(token_name n);
    token lexRelationalOp(int len, relation_op op);
    token lexArithmeticOp(arithmetic_op op);
    token lexBitwiseOp(int len, bitwise_op op);
    token lexConditionalOp();
    token lexAssignmentOp();
    token lexWord();
    token lexNumber();
    token lexBinNum();
    token lexHexNum();
    token lexChar();
    token lexString();

    char scanEscSeq();
    

    symbol_table& symbols;

    const char* m_first;
    const char* m_last;

    location m_curr_loc;

    location m_tok_loc;

    std::unordered_map<symbol, token> m_res_words;

};


