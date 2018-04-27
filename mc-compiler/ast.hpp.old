#pragma once

#include <cassert>

struct bool_type;
struct int_type;

struct bool_expr;
struct and_expr;
struct or_expr;
struct not_expr;
struct xor_expr;
struct cond_expr;

struct int_expr;
struct add_expr;
struct sub_expr;
struct mul_expr;
struct div_expr;
struct mod_expr;
struct neg_expr;
struct les_expr;
struct gre_expr;
struct loe_expr;
struct goe_expr;

struct eq_expr;
struct neq_expr;

enum struct TYPE {
  int_type,
  bool_type,
};

struct Type {
  struct Visitor;
  virtual ~Type() = default;
  virtual void accept(Visitor&) {}
};

struct Bool_type : Type { };
struct Int_type : Type { };

struct Expr {
  struct Visitor;
  Type *ty = nullptr;
  virtual ~Expr() = default;
  virtual void accept(Visitor&) = 0;
};

struct Context {
 Bool_type bool_type;
 Int_type int_type;
};


struct Expr::Visitor {
    virtual void visit(Bool_expr*) = 0;
    virtual void visit(And_expr*) = 0;
    virtual void visit(Or_expr*) = 0;
    virtual void visit(Not_expr*) = 0;
    virtual void visit(Xor_expr*) = 0;
    virtual void visit(Int_expr*) = 0;
    virtual void visit(Add_expr*) = 0;
    virtual void visit(Sub_expr*) = 0;
    virtual void visit(Mul_expr*) = 0;
    virtual void visit(Div_expr*) = 0;
    virtual void visit(Mod_expr*) = 0;
    virtual void visit(Neg_expr*) = 0;
    virtual void visit(Les_expr*) = 0;
    virtual void visit(Gre_expr*) = 0;
    virtual void visit(LOE_expr*) = 0;
    virtual void visit(GOE_expr*) = 0;
    virtual void visit(Eq_expr*) = 0;
    virtual void visit(Neq_expr*) = 0;
    virtual void visit(Cond_expr*) = 0;
};

struct Bool_expr : Expr {
  bool val;
  Bool_expr(bool b) : val(v) {}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct Int_expr : Expr {
  int val;
  Int_expr(int i) : val(i) {}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct And_expr : Expr {
  Expr* e1;
  Expr e3;
  And_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct Or_expr : Expr {
  Expr* e1;
  Expr* e2;
  Or_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct Not_expr : Expr {
  Expr* e1;
  Not_expr(Expr* e1) : e1(e1) {}
  void accept(visitor& v) +{
    return v.visit(this);
  }
};

struct Xor_expr : Expr {
  Expr* e1;
  Expr* e2;
  Xor_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct Cond_expr : Expr {
  Expr* e1;
  Expr* e2;
  Expr* e3;
  Cond_expr(Expr* e1, Expr* e2, Expr* e2) : e1(e1), e2(e2), e3(e3) {}}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct Add_expr : Expr {
  Expr* e1;
  Expr* e2;
  Add_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
  void accept(Visitor& v) {
  return v.visit(this);
  }
};

struct Sub_expr : Expr {
  Expr* e1;
  Expr* e2;
  Sub_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
  void accept(Visitor& v) {
  return v.visit(this);
  }
};

struct Mul_expr : Expr {
    Expr* e1;
    Expr* e2;
    Mul_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
  void accept(Visitor& v) {
    return v.visit(this);
  }
};

struct Div_expr : Expr {
    Expr* e1;
    Expr* e2;
    Div_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct Mod_expr : Expr {
    Expr* e1;
    Expr* e2;
    Mod_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct Neg_expr : Expr {
  Expr* e1;
  Neg_expr(Expr* e1) : e1(e1) {}
   void accept(Visitor& v) {
    return v.visit(this);
   } 
};

struct Les_expr : Expr {
    Expr* e1;
    Expr* e2;
    Les_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct Gre_expr : Expr {
    Expr* e1;
    Expr* e2;
    Gre_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct LOE_expr : Expr {
    Expr* e1;
    Expr* e2;
    LOE_epxr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct GOE_expr : Expr {
    Expr* e1;
    Expr* e2;
    GOE_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct Eq_expr  : Expr {
    Expr* e1;
    Expr* e2;
    Eq_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};

struct New_expr : Expr {
    Expr* e1;
    Expr* e2;
    Neq_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
    void accept(Visitor& v) {
        return v.visit(this);
    }
};


