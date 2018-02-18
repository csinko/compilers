#include "token.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>

const char* toString(token_name n) {
  switch (n) {
    case tok_eof:
      return "eof";

    case tok_left_brace:
      return "left-brace";
    case tok_left_paren:
      return "left-paren";
    case tok_left_bracket:
        return "left-bracket";
    case tok_right_brace:
      return "right-brace";
    case tok_right_paren:
      return "right-paren";
    case tok_right_bracket:
      return "right-bracket";

    case tok_comma:
      return "comma";
    case tok_semicolon:
      return "semicolon";
    case tok_colon:
        return "colon";

    case tok_relational_operator:
        return "relational-operator";
    case tok_arithmetic_operator:
        return"arithmetic-operator";
    case tok_bitwise_operator:
        return "bitwise-operator";
    case tok_logical_operator:
        return "logical-operator";
    case tok_conditional_operator:
        return "conditional-operator";
    case tok_assignment_operator:
        return "assignemnt-operator";

    case kw_def:
        return "def";
    case kw_if:
        return "if";
    case kw_else:
        return "else";
    case kw_var:
        return "var";
    case kw_let:
        return "let";

    case tok_identifier:
        return "identifier";
    case tok_binary_integer:
        return "binary-int";
    case tok_decimal_integer:
        return "decimal-int";
    case tok_hexadecimal_integer:
        return "hexadecimal-int";
    case tok_boolean:
        return "boolean";
    case tok_floating_point:
        return "floating-point";
    case tok_char:
        return "character";
    case tok_string:
        return "string";
    case tok_type_specifier:
        return "type-specifier";
  }
}

const char* toString(relation_op op) {
  switch(op) {
    case op_eq:
      return "eq";
    case op_ne:
      return "ne";
    case op_lt:
      return "lt";
    case op_gt:
      return "gt";
    case op_le:
      return "le";
    case op_ge:
      return "ge";
  }
};

const char* toString(arithmetic_op op) {
  switch (op) {
    case op_add:
      return "add";
    case op_sub:
      return "sub";
    case op_mul:
      return "mul";
    case op_div:
      return "div";
    case op_mod:
      return "mod";
  }
};

const char * toString(bitwise_op op) {
  switch (op) {
    case op_and:
      return "and";
    case op_ior:
      return "ior";
    case op_xor:
      return "xor";
    case op_shl:
      return "shl";
    case op_shr:
      return "shr";
    case op_not:
      return "not";
  }
};

const char* toString(logical_op op) {
  switch (op) {
    case logical_and:
      return "and";
    case logical_or:
      return "or";
    case logical_not:
      return "not";
  }
};

const char* toString(type_spec ts) {
  switch (ts) {
    case ts_bool:
      return "bool";
    case ts_int:
      return "int";
    case ts_char:
      return "char";
    case ts_float:
      return "float";
  }
};

token::token() : m_name(tok_eof) {}


token::token(token_name n, token_attr a, location loc) : m_name(n), m_attr(a), m_loc(loc) {}

static bool has_attribute(token_name n) {
  switch(n) {
    default:
      return false;

    case tok_relational_operator:
    case tok_arithmetic_operator:
    case tok_bitwise_operator:
    case tok_logical_operator:
    case tok_identifier:
    case tok_binary_integer:
    case tok_decimal_integer:
    case tok_hexadecimal_integer:
    case tok_boolean:
    case tok_floating_point:
    case tok_char:
    case tok_string:
    case tok_type_specifier:
      return true;
  }
}

token::token(token_name n, location loc) : m_name(n), m_loc(loc) {
    assert(!has_attribute(n));
}

token::token(relation_op op, location loc) : m_name(tok_relational_operator), m_attr(op), m_loc(loc) {}

token::token(symbol sym, location loc) : m_name(tok_identifier), m_attr(sym), m_loc(loc) {}

token::token(arithmetic_op op, location loc) : m_name(tok_arithmetic_operator), m_attr(op), m_loc(loc) {}

token::token(bitwise_op op, location loc) : m_name(tok_bitwise_operator), m_attr(op), m_loc(loc) {}

token::token(logical_op op, location loc) : m_name(tok_logical_operator), m_attr(op), m_loc(loc) {}

token::token(long long val, location loc) : token(tok_decimal_integer, decimal, val, loc) {}

static token_name get_token_name(radix rad) {
    switch (rad) {
      case binary:
        return tok_binary_integer;
      case decimal:
        return tok_decimal_integer;
      case hexadecimal:
        return tok_hexadecimal_integer;
    }
}

token::token(radix rad, long long val, location loc) : token(get_token_name(rad), rad, val, loc) {}

static bool check_radix(token_name n, radix rad) {
  switch (n) {
    case tok_binary_integer:
      return rad == binary;
    case tok_decimal_integer:
      return rad == decimal;
    case tok_hexadecimal_integer:
      return rad == hexadecimal;
    default:
      throw std::logic_error("token name not valid");
  }
}

token::token(token_name n, radix rad, long long val, location loc) : m_name(n), m_attr(integer_attr{rad, val}), m_loc(loc) {
  assert(check_radix(n, rad));
}

token::token(double val, location loc) : m_name(tok_floating_point), m_attr(val), m_loc(loc) {}

token::token(bool tf, location loc) : m_name(tok_boolean), m_attr(tf), m_loc(loc) {}

token::token(char c, location loc) : m_name(tok_char), m_attr(c), m_loc(loc) {}

token::token(string_attr s, location loc) : m_name(tok_string), m_attr(s), m_loc(loc) {}

token::token(type_spec ts, location loc) : m_name(tok_type_specifier), m_attr(ts), m_loc(loc) {}

static std::string escape(char c) {
  switch (c) {
    default: return {c};
    case '\'': return "\\\'";
    case '\"': return "\\\"";
    case '\\': return "\\\\";
    case '\a': return "\\a";
    case '\b': return "\\b";
    case '\f': return "\\f";
    case '\n': return "\\n";
    case '\r': return "\\n";
    case '\t': return "\\t";
    case '\v': return "\\v";
  }
}

static std::string escape (const std::string& s) {
  std::string result;
  for (char c : s) {
    result += escape(c);
  }
  return result;
}

std::ostream& operator<<(std::ostream& os, token tok) {
  os << '<';

  os << tok.getLocation() << ':';

  os << toString(tok.getName());
  switch (tok.getName()) {
    default:
      break;

    case tok_relational_operator:
      os << ':' << toString(tok.getRelationOp());
      break;

    case tok_arithmetic_operator:
      os << ':' << toString(tok.getArithmeticOp());
      break;
    
    case tok_bitwise_operator:
      os << ':' << toString(tok.getBitwiseOp());
      break;

    case tok_logical_operator:
      os << ':' << toString(tok.getLogicalOp());
      break;

    case tok_identifier:
      os << ':' << *tok.getIdentifier();
      break;

    case tok_binary_integer:
    case tok_decimal_integer:
    case tok_hexadecimal_integer:
      os << ':' << std::setbase(tok.getRadix()) << tok.getInteger();
      break;

    case tok_boolean:
      os << ':' << tok.getBoolean();
      break;
    
    case tok_floating_point:
      os << ':' << tok.getFloatingPoint();
      break;

    case tok_string:
      os << ':' << escape(tok.getString());
      break;

     case tok_type_specifier:
      os << ':' << toString(tok.getTypeSpecifier());
      break;
  }
  os << '>';
  return os;
}
