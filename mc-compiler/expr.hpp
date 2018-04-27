#pragma once

#include "token.hpp"

#include <vector>

class type;
class decl;

class expr {

  public:
    enum kind {
      bool_kind,
      int_kind,
      float_kind,
      id_kind,
      unop_kind,
      binop_kind,
      ptr_kind,
      call_kind,
      index_kind,
      cast_kind,
      assign_kind,
      cond_kind,
      conv_kind,
    };


  protected:
    expr(kind k) : m_kind(k), m_type() {}

    expr (kind k, type* t) : m_kind(k), m_type(t) {}

  public:
    virtual ~expr() = default;

    kind getKind() const {
      return m_kind;
    }

    type* getType() const { 
    return m_type;
    }

    type* getObjectType() const;

    bool hasType(const type* t) const;

    bool isBool() const;
    bool isInt() const;
    bool isFunction() const;
    bool isArithmetic() const;
    bool isNumeric() const;
    bool isScalar() const;

  private:
    kind m_kind;
    type* m_type;
};

using expr_list = std::vector<expr*>;


struct bool_expr : expr {
    bool_expr(type* t, bool b) : expr(bool_kind, t), val(b) {}

    bool val;
};

struct int_expr : expr {
    int_expr(type* t, int n) : expr(int_kind, t), val(n) {}

    int val;
};

struct float_expr : expr {
    float_expr(type* t, double n) : expr(float_kind, t), val(n) {}

    double val;
};


struct id_expr : expr {
    id_expr(type* t, decl* d) : expr(id_kind, t), ref(d) {}

    decl* ref;
};


enum unop {
    uo_pos,
    uo_neg,
    uo_cmp,
    uo_not,
    uo_addr,
    uo_deref,
};

struct unop_expr : expr {
  unop_expr(unop op, expr* e1) : expr(unop_kind), m_op(op), m_arg(e1) {}

  unop m_op;
  expr* m_arg;
};


enum binop {
  bo_add,
  bo_sub,
  bo_mul,
  bo_quo,
  bo_rem,
  bo_and,
  bo_ior,
  bo_xor,
  bo_shl,
  bo_shr,
  bo_land,
  bo_lor,
  bo_eq,
  bo_ne,
  bo_lt,
  bo_gt,
  bo_le,
  bo_ge,
};

struct binop_expr : expr {
  binop_expr(type* t, binop op, expr* e1, expr* e2) : expr(binop_kind,t), m_op(op), m_lhs(e1), m_rhs(e2) {}

  binop m_op;
  expr* m_lhs;
  expr* m_rhs;
};

struct postfix_expr : expr {
  postfix_expr(kind k, type* t, expr*e, const expr_list& args) : expr(k), m_base(e), m_args(args) {}

  expr* m_base;
  expr_list m_args;
};

struct call_expr : postfix_expr {
    call_expr(type* t, expr* e, const expr_list& args) : postfix_expr(call_kind, t, e, args) {}
};

struct index_expr : postfix_expr {
  index_expr(type* t, expr* e, const expr_list& args) : postfix_expr(index_kind, t, e, args) {}
};

struct cast_expr : expr {
  cast_expr(expr* e, type* t) : expr(cast_kind, t), m_src(e), m_dst(t) {}

  expr* m_src;
  type* m_dst;
};

struct assign_expr : expr {
  assign_expr(type* t, expr* e1, expr* e2) : expr(assign_kind), m_lhs(e1), m_rhs(e2) {}

  expr* m_lhs;
  expr* m_rhs;
};


struct cond_expr : expr {
  cond_expr(type* t, expr* e1, expr* e2, expr* e3) : expr(cond_kind, t), m_cond(e1), m_true(e2), m_false(e3) {}

  expr* m_cond;
  expr* m_true;
  expr* m_false;
};

enum conversion {
  conv_identity,
  conv_value,
  conv_bool,
  conv_char,
  conv_int,
  conv_ext,
  conv_trunc,
};

struct conv_expr : expr {
  conv_expr(expr* e1, conversion c, type* t) : expr(conv_kind, t), m_src(e1), m_conv(c) {}

  expr* m_src;
  conversion m_conv;
};
