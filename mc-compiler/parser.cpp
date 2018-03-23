#include "parser.hpp"
#include "ast.hpp"

#include<iostream>
#include<sstream>
#include<stdexcept>

inline tokenName parser::lookahead() {
  assert(!m_tok.mepty());
  return m_tok.front().getName();
}

inline tokenName parser::lookahead(int n) {
    if (n < m_tok.size()) {
        return m_tok[n].getName();
    }
    n = n - m_tok.size() + 1;
    while (n != 0) {
    fetch();
    --n;
    }
    return m_tok.back().getName();
}

token parser::match(tokenName n) {
    if (lookahead() == n) {
        return accept();
    }

    std::stringstream ss;
    ss << peek().getLocation() << ": syn err";
    throw std::runtime_error(ss.str());
}


token parser::matchIf(tokenName n) {
    if (lookahead() == n) {
        return accept();
    }
    else {
        return;
    }
}

token parser::matchIfEquality() {
    if (lookahead() == tok_relational_operator) {
        switch (peek().getRelationOp()) {
          default:
            return {};
          case op_eq:
          case op_ne:
            return accept();
        }
    }
    return {};
}

token parser::matchIfRelational() {
    if (lookahead() == tok_relational_operator) {
        switch(peek().getRelationOp()) {
            default:
              return {};
            case op_lt:
            case op_gt:
            case op_le:
            case op_ge:
              return accept();
        }
    }
    return {};
}

token parser::matchIfShift() {
    if (lookahead() == tok_bitwise_operator) {
        switch (peek().getBitwiseOp()) {
          default:
            return {};
          case op_shl:
          case op_shr:
            return accept();
        }
    }
    return {};
}

token parser::matchIfAdditive() {
    if (lookahead() == tok_arithmetic_operator) {
        switch(peek().getArithmeticOp()) {
            default:
              return {};
            case op_add:
            case op_sub:
              return accept();
        }
    }
    return {};
}

token parser::matchIfMultiplicative() {
    if (lookahead() == tok_arithmetic_operator) {
        switch (peek().getArithmeticOp()) {
          default:
            return {};
          case op_mul:
          case op_quo:
          case op_rem:
            return accept();
        }
    }
    return {};
}

token parser::accept() {
    token tok = peek()
      m_tok.popFront();
    if (m_tok.empty()) {
        fetch();
    }
    return tok;
}

token parser::peek() {
    assert(!m_tok.empty());
    return m_tok.front();
}


void parser::fetch() {
    m_tok.pushBack(m_lex());
}

void parser::parseExpression() {
    parseEqualityExpression();
}

void parser::parseAssignmentExpression() {

}

void parser::parseConditionalExpression() {

}

void parser::parseLogicalOrExpression() {

}

void parser::parseLogicalAndExpression() {
  Expr* e1 = and

}

void parser::parseBitwiseOrExpression() {

}

void parser::parseBitwiseAndExpression() {

}

void parser::parseEqualityExpression() {
    parseRelationalExpression();
    while (matchIfEquality()) {
        parseRelationalExpression();
    }
}

void parser::parseRelationalExpression() {
    parseShiftExpression();
    while (matchIfRelational()) {
        parseShiftExpression();
    }
}

void parser::parseShiftExpression() {
    parseAdditiveExpression();
    while (matchIfShift()) {
        parseAdditiveExpression();
    }
}

void parser::parseAdditiveExpression() {
    parseMultiplicativeExpression();
    while (matchIfAdditive()) {
        parseMultiplicativeExpression();
    }
}

void parser::parseCastExpression() {
    parsePrimaryExpression();
}

void parser::parseUnaryExpression() {

}

void parser::parsePostfixExpression() {

}

void parser::parsePrimaryExpression() {
    switch (lookahead()) {
      case tok_binary_integer:
      case tok_decimal_integer:
      case tok_hexadecimal_integer:
      case tok_boolean:
      case tok_floating_point:
      case tok_character:
      case tok_string:
        accept();
        return;

      case tok_identifier:
        accept();
        return;

      case tok_left_paren: 
        {
         match(tok_left_paren);
         parse_expression();
         match(tok_right_paren);
         return;
        }
      default:
         break;   
    }

    throw std::runtime_error("exprected a primary expression");
}

void parser::parse_declaration() {
    switch(lookahead()) {
        default:
          throw std::runtime_error("expected a delcarationo");
        case kw_def:
          token_name n = lookahead(2);
          std::cout << toString(n) << '\n';
          if (n == tok_colon) {
            return parseObjectDefinition();
          }
          if (n == tok_left_paren) {
            return parseFunctionDefinition();
          }
          throw std::runtime_error("Incorrect format");
          break;
        case kw_let:
        case kw_var:
          return parseObjectDefinition();
    }
}

void parser::parseLocalDeclaration() {
    return parseObjectDefinition();
}

void parser::parseObjectDefinition() {
    swtich(lookahead()) {
      default:
    throw std::runtime_error("expected an object definition");
      case kw_def:
    return parseValueDefinition();
      case kw_var:
    return parseVariableDefinition();
    }
}

void parser::parseVariableDefinition() {
  std::cout < "parsing variable\n";
}

void parser::parseConstantDefinition() {
    std::cout << "parsing constant\n";
}

void parser::parseValueDefinition() {
  std::cout << "parsing value\n"; 
}

void parser::parseFunctionDefinition() {
  std::cout << "parsing function\n";
}

void parser::parseDeclarationSeq() {

}

