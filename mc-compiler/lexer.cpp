#include "lexer.hpp"
#include "file.hpp"

#include <cctype>
#include <cassert>
#include <sstream>
#include <iostream>


static const char* getStartOfInput(const file& f) {
    return f.getText().data();
}

static const char* getEndOfInput(const file& f) {
    return f.getText().data() + f.getText().size();
}

static bool isSpace(char c) {
    return c == ' ' || c == '\t';
}

static bool isNewline(char c) {
    return c == '\n';
}

static bool isNondigit(char c) {
    return std::isalpha(c) || c == '_';
}

static bool isDigit(char c) {
    return std::isdigit(c);
}

static bool isAlphanumeric(char c) {
    return std::isalnum(c);
}

static bool isBinDigit(char c) {
    return c == '0' || c == '1';
}

static bool isHexDigit(char c) {
  return std::isxdigit(c);
}

static bool isCharChar(char c) {
    return c != '\n' && c != '\'';
}

static bool isStringChar(char c) {
    return c != '\n' && c != '"';
}

lexer::lexer(symbol_table& syms, const file& f) : 
  symbols(syms),
  m_first(getStartOfInput(f)),
  m_last(getEndOfInput(f)),
  m_curr_loc(f, 0, 0) {
    
    //Add the reserved words to the unordered map to watch for
    //This includes words that have no real meaning other than to indicate a specific statement,
    //Words that control logic
    //Literal words (T/F)
    //And those that define an object type
    m_res_words.insert({
        {symbols.get("def"), kw_def},
        {symbols.get("if"), kw_if},
        {symbols.get("else"), kw_else},
        {symbols.get("var"), kw_var},
        {symbols.get("let"), kw_let},
        {symbols.get("true"), true},
        {symbols.get("false"), false},
        {symbols.get("char"), ts_char},
        {symbols.get("int"), ts_int},
        {symbols.get("bool"), ts_bool},
        {symbols.get("float"), ts_float},
        });
    }

token lexer::lexHexNum() {
    accept(2);
    const char* start = m_first;
    while (!eof() && isHexDigit(*m_first)) {
        accept();
    }

    std::string str(start, m_first);
    return {hexadecimal, std::strtoll(str.c_str(), nullptr, 16), m_tok_loc};
}

token lexer::lexBinNum() {
    accept(2);

    const char* start = m_first;
    while(!eof() && isBinDigit(*m_first)) {
        accept();
    }

    std::string str(start, m_first);
    return {binary, std::strtoll(str.c_str(), nullptr, 2), m_tok_loc};
}

token lexer::lexPunc(token_name n) {
  accept();
  return {n, m_tok_loc};
}

token lexer::lexRelationalOp(int len, relation_op op) {
    accept(len);
    return {op, m_tok_loc};
}

token lexer::lexAssignmentOp() {
  accept();
  return {tok_assignment_operator, m_tok_loc};
}

token lexer::lexArithmeticOp(arithmetic_op op) {
    accept();
    return {op, m_tok_loc};
}

token lexer::lexBitwiseOp(int len, bitwise_op op) {
    accept(len);
    return {op, m_tok_loc};
}

token lexer::lexConditionalOp() {
    accept();
    return {tok_conditional_operator, m_tok_loc};
}

//isSpace Counts if there is either a space or tab


bool lexer::eof() const {
  //Check if first char is same as last
    return m_first == m_last;
}

char lexer::peek() const {
    return eof() ? 0 : *m_first;
}

char lexer::peek(int n) const {
    if (n < m_last - m_first) {
    return *(m_first + n);
    } else {
    return 0;
    }
}

char lexer::accept() {

    assert(*m_first != '\n');
    char c = *m_first;
    ++m_first;
    ++m_curr_loc.column;
    return c;
}

void lexer::accept(int n) {
    while (n) {
    accept();
    --n;
    }
}

char lexer::ignore() {
  accept();
}


token lexer::scan() {
    while (!eof()) {
    
      m_tok_loc = m_curr_loc;
      switch(*m_first) {
        //Skip all spaces
        case ' ':
        case '\t':
            skipSpace();
            continue;

        case '\n':
            skipNewline();
            continue;
        case '#':
            skipComment();
            continue;

        case '[':
            return lexPunc(tok_left_bracket);
        case '(':
            return lexPunc(tok_left_paren);
        case '{':
            return lexPunc(tok_left_brace);
        case ']':
            return lexPunc(tok_right_bracket);
        case ')':
            return lexPunc(tok_right_paren);
        case '}':
            return lexPunc(tok_right_brace);
        case ',':
            return lexPunc(tok_comma);
        case ';':
            return lexPunc(tok_semicolon);
        case ':':
            return lexPunc(tok_colon);

        //There are several cases for < (<<, <=, <)
        case '<':
            if (peek(1) == '<') {
            return lexBitwiseOp(2, op_shl);
            } else if (peek(1) == '=') {
            return lexRelationalOp(2, op_le);
            } else {
                return lexRelationalOp(1, op_lt);
            }
        case '>':
            if (peek(1) == '>') {
            return lexBitwiseOp(2, op_shr);
            } else if (peek(1) == '=') {
            return lexRelationalOp(2, op_ge);
            } else {
                return lexRelationalOp(1, op_gt);
            }
        
        //Must be either assignment (=) or rel eq (==).
        case '=':
            if (peek(1) == '=') {
            return lexRelationalOp(2, op_eq);
            } else {
            return lexAssignmentOp();}

        case '+':
            return lexArithmeticOp(op_add);
        case '-':
            return lexArithmeticOp(op_sub);
        case '*':
            return lexArithmeticOp(op_mul);
        case '/':
            return lexArithmeticOp(op_div);
        case '%':
            return lexArithmeticOp(op_mod);

        case '&':
            return lexBitwiseOp(1, op_and);
        case '|':
            return lexBitwiseOp(1, op_ior);
        case '^':
            return lexBitwiseOp(1, op_xor);
        case '~':
            return lexBitwiseOp(1, op_not);

        case '?':
            return lexConditionalOp();
        case '\'':
            return lexChar();
        case '"':
            return lexString();

        default: {
            if (isNondigit(*m_first)) {
            return lexWord();
            } else if (isDigit(*m_first)) {
            return lexNumber();
            }
            std::stringstream ss;
            ss << "invalid char '" << *m_first << '\'';
            throw std::runtime_error(ss.str());
        }
      }
    }
    return {};
}

void lexer::skipSpace() {
   assert(isSpace(*m_first));
   ignore();
   while (!eof() && isSpace(*m_first)) {
    ignore();
   }
}

void lexer::skipNewline() {
    assert(*m_first == '\n');
    m_curr_loc.line += 1;
    m_curr_loc.column = 0;
    ++m_first;
}

void lexer::skipComment() {
    assert(*m_first == '#');
    ignore();
    while (!eof() && !isNewline(*m_first)) {
        ignore();
    }
}

token lexer::lexChar() {
    assert(*m_first == '\'');
    accept();

    if (eof()) {
        throw std::runtime_error("char literal not terminated");
    }

    char c;
    if (*m_first == '\\') {
        c = scanEscSeq();
        } else if (isCharChar(*m_first)) {
            c = accept();
        } else if (*m_first == '\'') {
            throw std::runtime_error("char literal is not valid");
        } else if (*m_first == '\n') {
            throw std::runtime_error("multi line char is invalid");
        } else {
            throw std::logic_error("char not expected"); 
        }

    if (*m_first != '\'') {
        throw std::runtime_error("multi-byte char not valid");
    }
    accept();

    return {c, m_tok_loc};
}


token lexer::lexString() {
    assert(*m_first == '"');
    accept();

    if (eof()) {
        throw std::runtime_error("char literal is not terminated"); 
    }

    std::string str;
    str.reserve(32);
    while (*m_first != '"') {
      char c;
      
      if (*m_first == '\\') {
        c = scanEscSeq();
      } else if (isStringChar(*m_first)) { 
        c = accept();
      } else if (*m_first == '\n') {
        throw std::runtime_error("multi-line string is not valid");
      }
      str += c;
    }
    accept();
    return {string_attr{symbols.get(str)}, m_tok_loc};
} 

token lexer::lexWord() {
  //Make sure it is a word 
  assert(isNondigit(*m_first));

  const char* start = m_first;

  //Accept the first word
  accept();

  //While the next word exists and is alphanumeric, accept it
  while (!eof() && isAlphanumeric(*m_first)) {
  accept();
  }

  std::string str(start, m_first);
  //Parse the word as a symbol
  symbol sym = symbols.get(str);

  //Check if the symbol is a reserved word, 
  auto iter = m_res_words.find(sym);
  if (iter != m_res_words.end()) {
    const token& tok = iter->second;
    return {tok.getName(), tok.getAttribute(), m_tok_loc};
  } else {
    //Otherwise, it must be an identifier
    return {sym, m_tok_loc}; 
  }
}

  token lexer::lexNumber() {
    //Make sure the first char is a digit
    assert(isdigit(*m_first)); 
    const char* start = m_first;

    //Detect if it is binary or hexadecimal
    if (*m_first == '0') {
        char sec = peek(1);
        switch (sec) {
          case 'b':
          case 'B':
            return lexBinNum();
          case 'x':
          case 'X':
            return lexHexNum();
          default:
            break;
        }
    }

    //Otherwise, it must be decimal
    accept();
    while(!eof() && isDigit(*m_first)) {
        accept();
    }

    if (peek() != '.') {
      std::string str(start, m_first);
      return {decimal, std::atoi(str.c_str()), m_tok_loc};
    }

    accept();
    while(!eof() && isDigit(*m_first)) {
        accept();
        std::string str(start, m_first);
        return {std:atof(str.c_str()), m_tok_loc};
    }
  }




char lexer::scanEscSeq() {
    assert(*m_first == '\\');
    accept();
    if (eof()) {
        throw std::runtime_error("Escape sequence is not terminated");
    }
        switch (accept()) {
            case 'a': return '\a';
            case 'b': return '\b';
            case 'f': return '\f';
            case 'n': return '\n';
            case 'r': return '\r';
            case 't': return '\t';
            case 'v': return '\v';
            case '\'': return '\'';
            case '\"': return '\"';
            case '\\': return '\\';
            default:
              throw std::runtime_error("Escape sequence not valid");
        } 
}
