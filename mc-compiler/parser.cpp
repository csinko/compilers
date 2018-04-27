#include "parser.hpp"
#include "ast.hpp"

#include<iostream>
#include<sstream>
#include<stdexcept>

inline token_name parser::lookahead() {
  assert(!m_tok.empty());
  return m_tok.front().getName();
}

inline token_name parser::lookahead(int n) {
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

token parser::match(token_name n) {
    if (lookahead() == n) {
        return accept();
    }

    std::stringstream ss;
    ss << peek().getLocation() << ": syn err";
    throw std::runtime_error(ss.str());
}


token parser::matchIf(token_name n) {
    if (lookahead() == n) {
        return accept();
    }
    else {
        return {};
    }
}

token parser::matchIfLogicalOr() {
  if (lookahead() == tok_logical_operator) {
    if (peek().getLogicalOp() == logical_or) {
      return accept();
    }
  }
  return {}; 
}

token parser::matchIfLogicalAnd() {
  if (lookahead() == tok_logical_operator) {
    if (peek().getLogicalOp() == logical_and) {
      return accept();
    }
  }
  return {};
}

token parser::matchIfBitwiseOr() {
  if (lookahead() == tok_bitwise_operator) {
    if (peek().getBitwiseOp() == op_ior) {
      return accept();
    }
  }
  return {};
}

token parser::matchIfBitwiseXor() {
  if (lookahead() == tok_bitwise_operator) {
    if (peek().getBitwiseOp() == op_xor) {
      return accept();
    }
  }
  return {};
}

token parser::matchIfBitwiseAnd() {
  if (lookahead() == tok_bitwise_operator) {
    if (peek().getBitwiseOp() == op_and) {
      return accept();
    }
  }
  return {};
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

type* parser::parseType() {
  return parseBasicType();
}

type* parser::parseBasicType() {
  switch(lookahead()) {
    case tok_type_specifier:
      return m_act.onBasicType(accept());

    case tok_left_paren: {
     match(tok_left_paren);
     type* t = parseType();
     match(tok_right_paren);
     return t;
   }
    default:
    throw std::runtime_error("Unexpected token type"); 
  }
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
          case op_div:
          case op_mod:
            return accept();
        }
    }
    return {};
}

token parser::accept() {
    token tok = peek();
      m_tok.pop_front();
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
    m_tok.push_back(m_lex());
}

expr* parser::parseExpression() {
  return parseAssignmentExpression();
}

expr* parser::parseAssignmentExpression() {
    expr* e1 = parseConditionalExpression();
    if (matchIf(tok_assignment_operator)) {
      expr* e2 = parseAssignmentExpression();
      return m_act.onAssignmentExpression(e1, e2);
    }
    return e1;
}

expr* parser::parseConditionalExpression() {
  expr* e1 = parseLogicalOrExpression();
  if(matchIf(tok_conditional_operator)) {
    expr* e2 = parseExpression();
    match(tok_colon);
    expr* e3 = parseConditionalExpression();
    return m_act.onConditionalExpression(e1, e2, e3);
  }
  return e1;
}

expr* parser::parseLogicalOrExpression() {
    expr* e1 = parseLogicalAndExpression();
    while (matchIfLogicalOr()) {
    expr* e2 = parseLogicalAndExpression();
    e1 = m_act.onLogicalOrExpression(e1, e2);
    }
    return e1;
}

expr* parser::parseLogicalAndExpression() {
  expr* e1 = parseBitwiseOrExpression();
  while (matchIfLogicalAnd()) {
    expr* e2 = parseBitwiseOrExpression();
    e1 = m_act.onLogicalAndExpression(e1, e2);
  }
  return e1;
}

expr* parser::parseBitwiseOrExpression() {
  expr* e1 = parseBitwiseXorExpression();
  while (matchIfBitwiseOr()) {
    expr* e2 = parseBitwiseXorExpression();
    e1 = m_act.onBitwiseOrExpression(e1, e2);
  }
  return e1;
}

expr* parser::parseBitwiseXorExpression() {
    expr* e1 = parseBitwiseAndExpression();
    while (matchIfBitwiseXor()) {
      expr* e2 = parseBitwiseAndExpression();
      e1 = m_act.onBitwiseXorExpression(e1, e2);
    }
    return e1;
}

expr* parser::parseBitwiseAndExpression() {
  expr* e1  = parseEqualityExpression();
  while (matchIfBitwiseAnd()) {
    expr* e2 = parseEqualityExpression();
    e1 = m_act.onBitwiseAndExpression(e1, e2);
  }
  return e1;
}


expr* parser::parseEqualityExpression() {
  expr* e1 = parseRelationalExpression();
  while (token tok = matchIfEquality()) {
    expr* e2 = parseRelationalExpression();
    e1 = m_act.onEqualityExpression(tok, e1, e2);
  }
  return e1;
}

expr* parser::parseRelationalExpression() {
  expr* e1 = parseShiftExpression();
  while (token tok = matchIfRelational()) {
    expr* e2 = parseShiftExpression();
    e1 = m_act.onRelationalExpression(tok, e1, e2);
  }
  return e1;
}

expr* parser::parseShiftExpression() {
    expr* e1 = parseAdditiveExpression();
    while (token tok = matchIfShift()) {
        expr* e2 = parseAdditiveExpression();
        e1 = m_act.onShiftExpression(tok, e1, e2);
    }
    return e1;
}

expr* parser::parseAdditiveExpression() {
  expr* e1 = parseMultiplicativeExpression();
  while (token tok = matchIfAdditive()) {
    expr* e2 = parseMultiplicativeExpression();
    e1 = m_act.onAdditiveExpression(tok, e1, e2);
  }
  return e1;
}

expr* parser::parseMultiplicativeExpression() {
    expr* e1 = parseCastExpression();
    while (token tok = matchIfMultiplicative()) {
        expr* e2 = parseCastExpression();
        e1 = m_act.onMultiplicativeExpression(tok, e1, e2);
    }
    return e1;
}

expr* parser::parseCastExpression() {
  expr* e = parseUnaryExpression();
  if (matchIf(kw_as)) {
    type* t = parseType();
    return m_act.onCastExpression(e, t);
  }
  return e;
}

expr* parser::parseUnaryExpression() {
  token op;
  switch (lookahead()) {
    case tok_arithmetic_operator:
      switch (peek().getArithmeticOp()) {
        case op_add:
        case op_sub:
        case op_mul:
          op = accept();
          break;
        default:
          break;
      }
    case tok_bitwise_operator:
      switch (peek().getBitwiseOp()) {
        case op_not:
        case op_and:
          op = accept();
          break;
        default:
          break;
      }
    case tok_logical_operator:
      if (peek().getLogicalOp() == logical_not) {
        op = accept();
      }
      break;
    default:
      break;
  }

  if (op) {
    expr* e = parseUnaryExpression();
    return m_act.onUnaryExpression(op, e);
  }

  return parsePostfixExpression();
}


expr* parser::parsePrimaryExpression() {
    switch (lookahead()) {
      case tok_binary_integer:
      case tok_decimal_integer:
      case tok_hexadecimal_integer:
        return m_act.onIntegerLiteral(accept());
      case tok_boolean:
        return m_act.onBooleanLiteral(accept());
      case tok_floating_point:
        return m_act.onFloatLiteral(accept());
      case tok_char:
      case tok_string:
        throw std::logic_error("String/Char not implemented in this version of the parser");

      case tok_identifier:
        return m_act.onIdExpression(accept());

      case tok_left_paren: 
        {
         match(tok_left_paren);
         expr* e = parseExpression();
         match(tok_right_paren);
         return e;
        }
      default:
        throw std::runtime_error("Was expecting a primary expression...");
    }

    throw std::runtime_error("exprected a primary expression");
}

expr_list parser::parseArgumentList() {
    expr_list args;
    while (true) {
        expr* arg = parseExpression();
        if (matchIf(tok_comma)) {
            continue;
        }
        if (lookahead() == tok_right_paren) {
            break;
        }
        if (lookahead() == tok_right_brace) {
            break;
        }
    }
    return args;
}

expr* parser::parsePostfixExpression() {
  expr* e = parsePrimaryExpression();
  while (true) {
    if (matchIf(tok_left_paren)) {
        expr_list args = parseArgumentList();
        match(tok_right_paren);
        e = m_act.onCallExpression(e, args);
    }
    else if (matchIf(tok_left_bracket)) {
        expr_list args = parseArgumentList();
        match(tok_right_bracket);
        e = m_act.onIndexExpression(e, args);
    }
    else {
        break;
    }
  }
  return e;
}


stmt* parser::parseStatement() {
    switch (lookahead()) {
        case kw_if:
          return parseIfStatement();
        case kw_while:
          return parseWhileStatement();
        case kw_break:
          return parseBreakStatement();
        case kw_continue:
          return parseContinueStatement();
        case kw_return:
          return parseReturnStatement();
        case kw_var:
        case kw_let:
        case kw_def:
          return parseDeclarationStatement();
        case tok_left_brace:
          return parseBlockStatement();
        default:
          return parseExpressionStatement();
    }
}

stmt* parser::parseBlockStatement() {
    match(tok_left_brace);
    m_act.enterBlockScope();
    m_act.startBlock();

    stmt_list ss;
    if (lookahead() != tok_right_brace) {
        ss = parseStatementSeq();
    }

    m_act.finishBlock();
    m_act.leaveScope();
    match(tok_right_brace);
    return m_act.onBlockStatement(ss);
}

stmt* parser::parseIfStatement() {
    assert(lookahead() == kw_if);
    accept();
    match(tok_left_paren);
    expr* e = parseExpression();
    match(tok_right_paren);
    stmt* t = parseStatement();
    match(kw_else);
    stmt* f = parseStatement();
    return m_act.onIfStatement(e, t, f);
}


stmt* parser::parseWhileStatement() {
    assert(lookahead() == kw_while);
    accept();
    match(tok_left_paren);
    expr* e = parseExpression();
    match(tok_right_paren);
    stmt* b = parseStatement();
    return m_act.onWhileStatement(e, b);
}

stmt* parser::parseBreakStatement() {
    assert(lookahead() == kw_break);
    accept();
    match(tok_semicolon);
    return m_act.onBreakStatement();
}

stmt* parser::parseContinueStatement() {
    assert(lookahead() == kw_continue);
    accept();
    match(tok_semicolon);
    return m_act.onContinueStatement();
}

stmt* parser::parseReturnStatement() {
    assert(lookahead() == kw_return);
    accept();
    expr* e = parseExpression();
    match(tok_semicolon);
    return m_act.onReturnStatement(e);
}


stmt* parser::parseDeclarationStatement() {
    decl* d = parseLocalDeclaration();
    return m_act.onDeclarationStatement(d);
}

stmt* parser::parseExpressionStatement() {
    expr* e = parseExpression();
    match(tok_semicolon);
    return m_act.onExpressionStatement(e);
}

stmt_list parser::parseStatementSeq() {
    stmt_list sl;
    while (true) {
        stmt* s = parseStatement();
        sl.push_back(s);
        if(lookahead() == tok_right_brace) {
            break;
        }
    }
    return sl;
}

decl* parser::parseDeclaration() {
    switch(lookahead()) {
        default:
          throw std::runtime_error("expected a delcaration");
        case kw_def: {
          token_name n = lookahead(2);
          if (n == tok_colon) {
            return parseObjectDefinition();
          }
          if (n == tok_left_paren) {
            return parseFunctionDefinition();
          }
          throw std::runtime_error("Incorrect format");
          }
        case kw_let:
        case kw_var:
          return parseObjectDefinition();
    }
    return nullptr;
}

decl* parser::parseLocalDeclaration() {
    return parseObjectDefinition();
}

decl* parser::parseObjectDefinition() {
    switch(lookahead()) {
      default:
    throw std::runtime_error("expected an object definition");
      case kw_def:
    return parseValueDefinition();
      case kw_var:
    return parseVariableDefinition();
    }
}

decl* parser::parseVariableDefinition() {
    assert(lookahead() == kw_var);
    accept();
    token id = match(tok_identifier);
    match(tok_colon);
    type* t = parseType();

    decl* d = m_act.onVariableDeclaration(id, t);

    match(tok_assignment_operator);
    expr* e = parseExpression();
    match(tok_semicolon);

    return m_act.onVariableDefinition(d, e);
}

decl* parser::parseConstantDefinition() {
  assert(lookahead() == kw_let);
  accept();
  token id = match(tok_identifier);
  match(tok_colon);
  type* t = parseType();

  decl* d = m_act.onConstantDeclaration(id, t);
  match(tok_assignment_operator);
  expr* e = parseExpression();
  match(tok_semicolon);

  return m_act.onConstantDefinition(d, e);
}

decl* parser::parseValueDefinition() {
  assert(lookahead() == kw_def);
  accept();
  token id = match(tok_identifier);
  match(tok_colon);
  type* t = parseType();

  decl* d = m_act.onValueDeclaration(id, t);

  match(tok_assignment_operator);;
  expr* e = parseExpression();
  match(tok_semicolon);

  return m_act.onValueDefinition(d, e);
}

decl* parser::parseFunctionDefinition(){
  assert(lookahead() == kw_def);
  accept();
  token id = match(tok_identifier);
  match(tok_left_paren);

  m_act.enterParameterScope();
  decl_list parms;
  if (lookahead() != tok_right_paren) {
    parms = parseParameterClause();
  }
  m_act.leaveScope();
  match(tok_right_paren);
  match(tok_arrow_operator);
  type* t = parseType();

  decl* d = m_act.onFunctionDeclaration(id, parms, t);

  stmt* s = parseBlockStatement();


  return m_act.onFunctionDefinition(d, s);
}

decl_list parser::parseDeclarationSeq() {
    decl_list dl;
    while (peek()) {
        decl* d= parseDeclaration();
        dl.push_back(d);
    }
    return dl;
}

decl_list parser::parseParameterClause() {
    return parseParameterList();
}

decl_list parser::parseParameterList()  {
   decl_list parms;
   while (true) {
    parms.push_back(parseParameter());
    if (matchIf(tok_comma)) {
        continue;
    }
    else {
        break;
    }
   }
   return parms;
}

decl* parser::parseParameter() {
    token id = match(tok_identifier);
    match(tok_colon);
    type* t = parseType();
    return m_act.onParameterDeclaration(id, t);
}

decl* parser::parseProgram() {
    m_act.enterGlobalScope();
    decl_list dl = parseDeclarationSeq();
    m_act.leaveScope();
    return m_act.onProgram(dl);
}

