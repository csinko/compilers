#pragma once

#include "symbol.hpp"

#include <vector>

class type;
class fn_type;
class expr;
class stmt;

class decl { 
    public:
      enum kind {
        var_kind,
        const_kind,
        value_kind,
        fn_kind,
        prog_kind,
        parm_kind,
      };

    protected:
      decl(kind k, symbol sym) : m_kind(k), m_name(sym) {}

    public:
      virtual ~decl() = default;

      kind getKind() const {
        return m_kind;
      }

      bool isVariable() const {
        return m_kind == var_kind || m_kind == parm_kind;
      }

      symbol getName() const {

        return m_name;
      }


    private:
      kind m_kind;
      symbol m_name;
};


using decl_list = std::vector<decl*>;

struct prog_decl : decl {

  prog_decl(const decl_list& ds) : decl(prog_kind, nullptr), m_decls(ds) {}

  const decl_list& getDelcarations() const { 
    return  m_decls;
  }

  decl_list& getDeclarations() {
    return m_decls;
  }

  decl_list m_decls;
};

struct typed_decl : decl {

  protected:
    typed_decl (kind k, symbol sym, type* t) : decl (k, sym), m_type(t) {}

  public:
       type* getType() const {
         return m_type;
       }

       void setType(type* t) {
         m_type = t;
       }

  protected:
       type* m_type;
};

struct object_decl : typed_decl {

  protected:
    object_decl (kind k, symbol sym, type* t, expr* e) : typed_decl(k, sym, t), m_init(e) {}

  public:
    expr* getInit() const { 
        return m_init;
    }

    void setInit(expr* e) {
      m_init = e;
    }

  protected:
    expr* m_init;
};


struct var_decl : object_decl {
  var_decl(symbol sym, type* t, expr* e = nullptr) : object_decl(var_kind, sym, t, e) {}
};

struct const_decl : object_decl {
  const_decl(symbol sym, type* t, expr* e = nullptr) : object_decl(const_kind, sym, t, e) {}
};

struct value_decl : object_decl {
  value_decl(symbol sym, type* t, expr* e = nullptr) : object_decl(value_kind, sym, t, e) {}
};

struct parm_decl : object_decl {
  parm_decl(symbol sym, type* t) : object_decl(parm_kind, sym, t, nullptr) {}
};

struct fn_decl: typed_decl {
  fn_decl(symbol sym, type* t, const decl_list& parms, stmt* s = nullptr) : typed_decl(fn_kind, sym, t), m_parms(parms), m_body(s) {}

  const decl_list& getParameters() const {
    return m_parms;
  }

  fn_type* getType() const;

  type* getReturnType() const;

  stmt* getBody() const {
    return m_body;
  }
  void setBody(stmt* s) {
    m_body = s;
  }

  decl_list m_parms;
  stmt* m_body;
};
