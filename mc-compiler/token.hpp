#pragma once

#include "symbol.hpp"
#include "location.hpp"

#include <cassert>
enum token_name {

  tok_eof, //End of file token

  //Punctuators
  tok_left_brace,
  tok_right_brace,
  tok_left_paren,
  tok_right_paren,
  tok_left_bracket,
  tok_right_bracket,
  tok_comma,
  tok_semicolon,
  tok_colon,

  //Operators
  tok_relational_operator, // ==, !=, <, >, ...
  tok_arithmetic_operator, // +, -, ...
  tok_bitwise_operator,
  tok_logical_operator,
  tok_conditional_operator,
  tok_assignment_operator,
  tok_arrow_operator, 

  //Keywords
  kw_def,
  kw_if,
  kw_else,
  kw_var,
  kw_let,
  kw_as,
  kw_break,
  kw_continue,
  kw_return,
  kw_while,

  //Everything else
  tok_identifier,
  tok_decimal_integer,
  tok_hexadecimal_digit,
  tok_hexadecimal_integer,
  tok_binary_digit,
  tok_binary_integer,
  tok_boolean,
  tok_floating_point,
  tok_char,
  tok_string,
  tok_type_specifier,
};

enum relation_op {
  op_eq,
  op_ne,
  op_lt,
  op_gt,
  op_le,
  op_ge,
};

enum arithmetic_op {
    op_add,
    op_mul,
    op_div,
    op_mod,
    op_sub,
};

enum bitwise_op {
  op_and,
  op_ior,
  op_xor,
  op_shl,
  op_shr,
  op_not,
};

enum logical_op {
  logical_and,
  logical_or,
  logical_not,
};

enum pointer_op {
    op_addr,
    op_deref,
};

enum type_spec {
  ts_char,
  ts_bool,
  ts_int,
  ts_float,
};

enum radix {
  binary = 2,
  decimal = 10,
  hexadecimal = 16,
};

const char* toString(token_name n);
const char* getSpelling(token_name n);
const char* toString(relation_op op);
const char* toString(arithmetic_op op);
const char* toString(bitwise_op op);
const char* toString(logical_op op);
const char* toString(type_spec ts);


struct integer_attr {
  radix rad;
  long long value;
};

struct string_attr {
  symbol sym;
};


union token_attr {
  token_attr() = default;
  token_attr(symbol sym) : sym(sym) {}
  token_attr(relation_op op) : relop(op) {}
  token_attr(arithmetic_op op) : arithop(op) {}
  token_attr(bitwise_op op) : bitop(op) {}
  token_attr(logical_op op) : logicop(op) {}
  token_attr(integer_attr val) : intval(val) {}
  token_attr(double val) : fpval(val) {}
  token_attr(bool tf) : truthval(tf) {}
  token_attr(char c) : charval(c) {}
  token_attr(string_attr s) : strval(s) {}
  token_attr(type_spec ts) : ts(ts) {}

  symbol sym;
  relation_op relop;
  arithmetic_op arithop;
  bitwise_op bitop;
  logical_op logicop;
  integer_attr intval;
  double fpval;
  bool truthval;
  char charval;
  string_attr strval;
  type_spec ts;
};

class token {
  public:
    token();
    token(token_name n, location loc = {});
    token(token_name n, token_attr attr, location loc = {});
    token(symbol sym, location los = {});
    token(relation_op op, location loc = {});
    token(arithmetic_op op, location loc = {});
    token(bitwise_op op, location loc = {});
    token(logical_op op, location loc = {});
    token(long long val, location loc = {});
    token(radix rad, long long val, location loc = {});
    token(token_name n, radix rad, long long val, location loc = {});
    token(double val, location loc = {});
    token(bool tf, location loc = {});
    token(char c, location loc = {});
    token(string_attr s, location loc = {});
    token(type_spec ts, location loc = {});

    // Returns true if is not EOF token
    operator bool() const {return m_name != tok_eof; }

    token_name getName() const {
      return m_name;
    }

    token_attr getAttribute() const {
      return m_attr;
    }

    location getLocation() const {
        return m_loc;
    }

    bool isIdentifier() const {
        return m_name == tok_identifier;
    }
    bool isInteger() const;
    bool isFloatingPoint() const;

    symbol getIdentifier() const;
    relation_op getRelationOp() const;
    arithmetic_op getArithmeticOp() const;
    bitwise_op getBitwiseOp() const;
    logical_op getLogicalOp() const;
    long long getInteger() const;
    double getFloatingPoint() const;
    radix getRadix() const;
    bool getBoolean() const;
    char getChar() const;
    const std::string& getString() const;
    type_spec getTypeSpecifier() const;

  private:
    token_name m_name;
    token_attr m_attr;
    location m_loc;
};

inline bool token::isInteger() const {
  return tok_binary_integer <= m_name || m_name <= tok_hexadecimal_integer || m_name <= tok_decimal_integer;
}

inline bool token::isFloatingPoint() const {
    return m_name == tok_floating_point;
}

inline symbol token::getIdentifier() const {
    assert(m_name == tok_identifier);
    return m_attr.sym;
}

inline relation_op token::getRelationOp() const {
    assert(m_name == tok_relational_operator);
    return m_attr.relop;
}

inline arithmetic_op token::getArithmeticOp() const {
    assert(m_name == tok_arithmetic_operator);
    return m_attr.arithop;
}

inline bitwise_op token::getBitwiseOp() const {
    assert(m_name == tok_bitwise_operator);
    return m_attr.bitop;
}

inline logical_op token::getLogicalOp() const {
    assert(m_name == tok_logical_operator);
    return m_attr.logicop;
}

inline long long token::getInteger() const {
    assert(isInteger());
    return m_attr.intval.value;
}

inline radix token::getRadix() const {
    assert(isInteger());
    return m_attr.intval.rad;
}

inline double token::getFloatingPoint() const {
    assert(m_name == tok_floating_point);
    return m_attr.fpval;
}

inline bool token::getBoolean() const {
    assert(m_name == tok_boolean);
    return m_attr.truthval;
}

inline char token::getChar() const {
    assert(m_name == tok_char);
    return m_attr.charval;
}

inline const std::string& token::getString() const {
    assert(m_name == tok_string);
    return *m_attr.strval.sym;
}

inline type_spec token::getTypeSpecifier() const {
    assert(m_name == tok_type_specifier);
    return m_attr.ts;
}

std::ostream& operator<<(std::ostream& os, token tok);
