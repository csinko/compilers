#include "semantics.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "decl.hpp"
#include "scope.hpp"


#include <iostream>
#include <sstream>

semantics::semantics() : m_scope(nullptr),m_fn(nullptr), m_bool(new bool_type()), m_char(new char_type()), m_int(new int_type()), m_float(new float_type()) {}

semantics::~semantics() {

  assert(!m_scope);
  assert(!m_fn);
}

type* semantics::onBasicType(token tok) {

  switch (tok.getTypeSpecifier()) {
    case ts_bool:
      return m_bool;
    case ts_char:
      return m_char;
    case ts_int:
      return m_int;
    case ts_float:
        return m_float;
  }
}

expr* semantics::onAssignmentExpression(expr* e1, expr* e2) {
  e1 = requireReference(e1);
  e2 = requireValue(e2);

  type* t1 = e1->getObjectType();
  type* t2 = e2->getType();
  requireSame(t1, t2);

  return new assign_expr(e1->getType(), e1, e2);
}

expr* semantics::onConditionalExpression(expr* e1, expr* e2, expr* e3) {
    e1 = requireBoolean(e1);

    type* c = commonType(e1->getType(), e2->getType());
    e2 = convertToType(e2, c);
    e3 = convertToType(e3, c);

    return new cond_expr(c, e1, e2, e3);
}

expr* semantics::onLogicalOrExpression(expr* e1, expr* e2) {
    e1 = requireBoolean(e1);
    e2 = requireBoolean(e2);
    return new binop_expr(m_bool, bo_lor, e1, e2);
}

expr* semantics::onLogicalAndExpression(expr* e1, expr* e2) {
    e1 = requireBoolean(e1);
    e2 = requireBoolean(e2);

    return new binop_expr(m_bool, bo_land, e1, e2);
}

expr* semantics::onBitwiseOrExpression(expr* e1, expr* e2) {
  e1 = requireInteger(e1);
  e2 = requireInteger(e2);
  return new binop_expr(m_int, bo_ior, e1, e2);
}

expr* semantics::onBitwiseXorExpression(expr* e1, expr* e2) {
    e1 = requireInteger(e1);
    e2 = requireInteger(e2);
    return new binop_expr(m_int, bo_xor, e1, e2);
}

expr* semantics::onBitwiseAndExpression(expr* e1, expr* e2) {
    e1 = requireInteger(e1);
    e2 = requireInteger(e2);
    return new binop_expr(m_int, bo_and, e1, e2);
}

static binop getRelationOp(relation_op op) {
 switch (op) {
   case op_eq:
     return bo_eq;
   case op_ne:
     return bo_ne;
   case op_lt:
     return bo_lt;
   case op_gt:
     return bo_gt;
   case op_le:
     return bo_le;
   case op_ge:
     return bo_ge;
 }
}


expr* semantics::onEqualityExpression(token tok, expr* e1, expr* e2) {

    e1 = requireScalar(e1);
    e2 = requireScalar(e2);
    relation_op op = tok.getRelationOp();
    return new binop_expr(m_bool, getRelationOp(op), e1, e2);
}


expr* semantics::onRelationalExpression(token tok, expr* e1, expr* e2) {

    e1 = requireNumeric(e1);
    e2 = requireNumeric(e2);
    relation_op op = tok.getRelationOp();
    return new binop_expr(m_bool, getRelationOp(op), e1, e2);
}

static binop getBitwiseOp(bitwise_op op) {
    switch (op) {
      case op_and:
        return bo_and;
      case op_ior:
        return bo_ior;
      case op_xor: 
        return bo_xor;
      case op_shl:
        bo_shl;
      case op_shr:
        return bo_shr;
      default:
        throw std::logic_error("Not a valid op");
    }
}

expr* semantics::onShiftExpression(token tok, expr* e1, expr* e2) {
    e1 = requireInteger(e1);
    e2 = requireInteger(e2);
    bitwise_op op = tok.getBitwiseOp();
    return new binop_expr(m_int, getBitwiseOp(op), e1, e2);
}

static binop getArithmeticOp(arithmetic_op op) {
    switch (op) {
        case op_add:
          return bo_add;
        case op_sub:
          return bo_sub;
        case op_mul:
          return bo_mul;
        case op_div:
          return bo_quo;
        case op_mod:
          return bo_rem;
    }
}

expr* semantics::onAdditiveExpression(token tok, expr* e1, expr* e2) {
    e1 = requireArithmetic(e1);
    e2 = requireArithmetic(e2);
    type* t = requireSame(e1->getType(), e2->getType());

    arithmetic_op op = tok.getArithmeticOp();
    return new binop_expr(t, getArithmeticOp(op), e1, e2);
}

expr* semantics::onMultiplicativeExpression(token tok, expr* e1, expr* e2) {
    e1 = requireArithmetic(e1);
    e2 = requireArithmetic(e2);
    type* t = requireSame(e1->getType(), e2->getType());

    arithmetic_op op = tok.getArithmeticOp();
    return new binop_expr(t, getArithmeticOp(op), e1, e2);
}

expr* semantics::onCastExpression(expr* e, type* t) {
    return new cast_expr(convertToType(e, t), t);
}

static unop getUnaryOp(token tok) {
    switch (tok.getName()) {
      case tok_arithmetic_operator:
        if (tok.getArithmeticOp() == op_add)
          return uo_pos;
        else if (tok.getArithmeticOp() == op_sub)
          return uo_neg;
        else
          throw std::logic_error("Not a valid op");
      case tok_bitwise_operator:
        if (tok.getBitwiseOp() == op_not)
          return uo_cmp;
        else
          throw std::logic_error("Not a valid op");
      case tok_logical_operator:
        if (tok.getLogicalOp())
          return uo_not;
        else
          throw std::logic_error("not a valid op");
      default:
        throw std::logic_error("Not a valid token");
    }
}

expr* semantics::onUnaryExpression(token tok, expr* e) {
    unop op = getUnaryOp(tok);
    type* t;
    switch (op) {
      case uo_pos:
      case uo_neg:
        e = requireArithmetic(e);
        t = e->getType();
        break;

      case uo_cmp:
        e = requireInteger(e);
        t = m_int;
        break;

      case uo_not:
        e = requireBoolean(e);
        t = m_bool;
        break;

      case uo_addr:
      case uo_deref:
        throw std::logic_error("Features not implemented in this compiler verison");
    }
    return new unop_expr(op, e);
}

expr* semantics::onCallExpression(expr* e, const expr_list& args) {
  e = requireFunction(e);
  fn_type* t = static_cast<fn_type*>(e->getType());

  type_list& parms = t->getParameterTypes();
  if (parms.size() < args.size())
    throw std::runtime_error("Too many Args");
  if (args.size() < parms.size())
    throw std::runtime_error("Not enough Args");

  //Args just right, like goldilocks
  for (std::size_t i = 0; i != parms.size(); ++i) {
    type* p = parms[i];
    expr* a = args[i];
    if (!a->hasType(p))
      throw std::runtime_error("arg does not match");
  }

  return new call_expr(t->getReturnType(), e, args);
}

expr* semantics::onIndexExpression(expr* e, const expr_list& args) {
    throw std::runtime_error("not implemented in this compiler version");
}

expr* semantics::onIntegerLiteral(token tok) {
    int val = tok.getInteger();
    return new int_expr(m_int, val);
}

expr* semantics::onBooleanLiteral(token tok) {
    int val = tok.getInteger();
    return new bool_expr(m_bool, val);
}

expr* semantics::onFloatLiteral(token tok) {
    int val = tok.getInteger();
    return new float_expr(m_float, val);
}

expr* semantics::onIdExpression(token tok) {
    symbol sym = tok.getIdentifier();

    decl* d = lookup(sym);
    if (!d) {
      std::stringstream ss;
      ss << "Ther eis not a matching decl for '" << *sym << "'";
      throw std::runtime_error(ss.str());
    }
    type * t;
    typed_decl* td = dynamic_cast<typed_decl*>(d);
    if (td->isVariable())
      t = new ref_type(td->getType());
    else
      t = td->getType();

    return new id_expr(t, d);
}

stmt* semantics::onBlockStatement(const stmt_list& ss) {
    return new block_stmt(ss);
}

void semantics::startBlock() {
    scope* parent = getCurrentScope()->parent;
    if (dynamic_cast<global_scope*>(parent)) {
        fn_decl* fn = getCurrentFunction();
        for (decl* parm : fn->m_parms)
            declare(parm);
    }
}

void semantics::finishBlock() {

}

stmt* semantics::onIfStatement(expr* e, stmt* s1, stmt* s2) {
    return new if_stmt(e, s1, s2);
}

stmt* semantics::onWhileStatement(expr* e, stmt* s) {
    return new while_stmt(e, s);
}

stmt* semantics::onBreakStatement() {
    return new break_stmt();
}

stmt* semantics::onContinueStatement() {
    return new cont_stmt();
}

stmt* semantics::onReturnStatement(expr* e) {
    return new ret_stmt(e);
}

stmt* semantics::onDeclarationStatement(decl* d) {
    return new decl_stmt(d);
}

stmt* semantics::onExpressionStatement(expr* e) {
    return new expr_stmt(e);
}


void semantics::declare(decl* d) {
    scope* s = getCurrentScope();
    if (s->lookup(d->getName())) {
        std::stringstream ss;
        ss << "There is a redecl of " << *d->getName();
        throw std::runtime_error(ss.str());
    }
    s->declare(d->getName(), d);
}

decl* semantics::onVariableDeclaration(token n, type* t) {
    decl* var = new var_decl(n.getIdentifier(), t);
    declare(var);
    return var;
}

decl* semantics::onVariableDefinition(decl* d, expr* e) {
    var_decl* var = static_cast<var_decl*>(d);
    var->setInit(e);
    return var;
}

decl* semantics::onConstantDeclaration(token n, type* t) {
    decl* var = new const_decl(n.getIdentifier(), t);
    declare(var);
    return var;
}

decl* semantics::onConstantDefinition(decl* d, expr* e) {
    const_decl* var = static_cast<const_decl*>(d);
    var->setInit(e);
    return var;
}

decl* semantics::onValueDeclaration(token n, type* t) {
    decl* val = new value_decl(n.getIdentifier(), t);
    declare(val);
    return val;
}

decl* semantics::onValueDefinition(decl* d, expr* e) {
    value_decl* val = static_cast<value_decl*>(d);
    val->setInit(e);
    return val;
}

decl* semantics::onParameterDeclaration(token n, type* t) {
    decl* parm = new parm_decl(n.getIdentifier(), t);
    declare(parm);
    return parm;
}

static type_list getParameterTypes(const decl_list& parms) {
    type_list types;
    for (const decl *d : parms) {
        types.push_back(static_cast<const parm_decl*>(d)->getType());
    }
    return types;
}

decl* semantics::onFunctionDeclaration(token n, const decl_list& parms, type* ret) {
    fn_type* ty = new fn_type(getParameterTypes(parms), ret);
    fn_decl* fn = new fn_decl(n.getIdentifier(), ty, parms);
    fn->setType(ty);
    declare(fn);

    assert(!m_fn);
    m_fn = fn;

    return fn;
}

decl* semantics::onFunctionDefinition(decl* d, stmt* s) {
    fn_decl* fn = static_cast<fn_decl*>(d);
    fn->setBody(s);

    assert(m_fn == fn);
    m_fn = nullptr;

    return fn;
}


decl* semantics::onProgram(const decl_list& dl) {
    return new prog_decl(dl);
}

void semantics::enterGlobalScope() {
    assert(!m_scope);
    m_scope = new global_scope();
}

void semantics::enterParameterScope() {
    m_scope = new parameter_scope(m_scope);
}

void semantics::enterBlockScope() {
    m_scope = new block_scope(m_scope);
}

void semantics::leaveScope() {
    scope* cur = m_scope;
    m_scope = cur->parent;
    delete cur;
}

decl* semantics::lookup(symbol n) {
    scope* s = getCurrentScope();
    while(s) {
        if (decl* d = s->lookup(n)) {
            return d;
        }
        s = s->parent;
    }
    return nullptr;
}

expr* semantics::requireReference(expr* e) {
    type* t = e->getType();
    if (!t->isReference()) {
        throw std::runtime_error("I dont see a ref");
    }
    return e;
}

expr* semantics::requireValue(expr* e) {
    return convertToValue(e);
}

expr* semantics::requireArithmetic(expr* e) {
    e = requireValue(e);
    if (!e->isArithmetic()) {
        throw std::runtime_error("I dont see a arith expr");
    }
    return e;
}


expr* semantics::requireNumeric(expr* e) {
    e = requireValue(e);
    if (!e->isNumeric()) {
        throw std::runtime_error("I dont see an arith expr");
    }
    return e;
}


expr* semantics::requireScalar(expr* e) {
    e = requireValue(e);
    if (!e->isScalar()) {
        throw std::runtime_error("Was looking for a scalar expr");
    }
    return e;
}

expr* semantics::requireInteger(expr* e) {
    e = requireValue(e);
    if (!e->isInt()) {
        throw std::runtime_error("No int expr found here");
    }
    return e;
}

expr* semantics::requireBoolean(expr* e) {
    e = requireValue(e);
    if (!e->isBool()) {
        throw std::runtime_error("Was expecting a boolean expression");
    }
    return e;
}

expr* semantics::requireFunction(expr* e) {
  e = requireValue(e);
  if (!e->isFunction()) {
    throw std::runtime_error("Did not find function");
  }
  return e;
}

type* semantics::requireSame(type* t1, type* t2) {
    if (!isSameAs(t1, t2)) {
        throw std::runtime_error("Types are not the same");
    }
    return t1;
}

type* semantics::commonType(type* t1, type* t2) {
    if (isSameAs(t1, t2)) {
        return t1;
    }
    if (t1->isReferenceTo(t2)) {
        return t2;
    }
    if (t2->isReferenceTo(t1)) {
        return t1;
    }

    throw std::runtime_error("Could not find a common type");
}

expr* semantics::convertToValue(expr* e) {
    type* t = e->getType();
    if (t->isReference()) {
        return new conv_expr(e, conv_value, t->getObjectType());
    }
    return e;
}


expr* semantics::convertToBool(expr* e) {
    e = convertToValue(e);
    type* t = e->getType();
    switch (t->getKind()) {
        case type::bool_kind:
          return e;
        case type::char_kind:
        case type::int_kind:
        case type::float_kind:
        case type::ptr_kind:
        case type::fn_kind:
          return new conv_expr(e, conv_bool, m_bool);
        default:
          throw std::runtime_error("Cannot convert type to boolean");
    }
}


expr* semantics::convertToChar(expr* e) {
    e = convertToValue(e);
    type* t = e->getType();
    switch (t->getKind()) {
        case type::char_kind:
          return e;
        case type::int_kind:
          return new conv_expr(e, conv_char, m_char);
        default:
          throw std::runtime_error("Cant conv to a char");
    }
}

expr* semantics::convertToInt(expr* e) {
    e = convertToValue(e);
    type* t = e->getType();
    switch (t->getKind()) {
        case type::bool_kind:
        case type::char_kind:
          return new conv_expr(e, conv_int, m_int);
        case type::int_kind:
          return e;
        case type::float_kind:
          return new conv_expr(e, conv_trunc, m_int);
        case type::ptr_kind:
        case type::fn_kind:
        default:
          throw std::runtime_error("Unable to convert to an int");
    }
}

expr* semantics::convertToFloat(expr* e) {
    e = convertToValue(e);
    type* t= e->getType();
    switch(t->getKind()) {
        case type::int_kind:
          return new conv_expr(e, conv_ext, m_float);
        case type::float_kind:
          return e;
        default:
          throw std::runtime_error("cannot convert to float");
    }
}

expr* semantics::convertToType(expr* e, type* t) {
    if (t->isObject()) {
        e = convertToValue(e);
    }

    if (e->hasType(t)) {
        return e;
    }


    switch (t->getKind()) {
        case type::bool_kind:
          return convertToBool(e);
        case type::char_kind:
          return convertToChar(e);
        case type::int_kind:
          return convertToInt(e);
        case type::float_kind:
          return convertToFloat(e);
        default:
          throw std::runtime_error("Failed to convert to type specified");
    }
}
