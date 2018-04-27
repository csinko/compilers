#pragma once

#include <vector>

class expr;
class decl;

class stmt {

  public:
    enum kind {
      block_kind,
      when_kind,
      if_kind,
      while_kind,
      break_kind,
      cont_kind,
      ret_kind,
      decl_kind,
      expr_kind,
    };

  protected:
    stmt(kind k) : m_kind(k) {}


  public:
    virtual ~stmt() = default;

    kind getKind() const {
      return m_kind;
    }

  private:
    kind m_kind;
};

using stmt_list = std::vector<stmt*>;

struct block_stmt : stmt {

  block_stmt(const stmt_list& ss) : stmt(block_kind), m_stmts(ss) {}

  const stmt_list& getStatements() const {
    return m_stmts;
  }

  stmt_list& getStatements() {
    return m_stmts;
  }

  stmt_list m_stmts;
};


struct when_stmt : stmt {

  when_stmt(expr* c, stmt* s1) : stmt(when_kind), m_cond(c), m_body(s1) {}

  expr* getCondition() const { 
  return m_cond;
  }

  stmt* getBody() const {
    return m_body;
  }

  expr* m_cond;
  stmt* m_body;
};

struct if_stmt : stmt {
    if_stmt(expr* c, stmt* s1, stmt* s2) : stmt(if_kind), m_cond(c), m_true(s1), m_false(s2) {}

    expr* getCondition() const {
      return m_cond;
    }

    stmt* getTrueBranch() const {
      return m_true;
    }

    stmt* getFalseBranch() const {
      return m_false;
    }

    expr* m_cond;
    stmt* m_true;
    stmt* m_false;
};

struct while_stmt : stmt {
    while_stmt(expr* c, stmt* s1) : stmt(while_kind), m_cond(c), m_body(s1) {}

  expr* getCondition() const {
      return m_cond;
  }

  stmt* getBody() const {
      return m_body;
  }

  expr* m_cond;
  stmt* m_body;
};

struct break_stmt : stmt {
    break_stmt() : stmt(break_kind) {}
};

struct cont_stmt : stmt {
    cont_stmt() : stmt(break_kind) {}
};

struct ret_stmt : stmt {
    ret_stmt(expr* e) : stmt(ret_kind), m_val(e) {}
    expr* m_val;
};

struct decl_stmt : stmt {
    decl_stmt(decl* d) : stmt(decl_kind), m_decl(d) {}

    decl* m_decl;
};

struct expr_stmt : stmt {
    expr_stmt(expr* e) : stmt(expr_kind), m_expr(e) {}

    expr* m_expr;
};
