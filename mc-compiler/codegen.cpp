#include "codegen.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "stmt.hpp"
#include "token.hpp"

#include <llvm/IR/LLBMContext.>h
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>



using vairable_map = std::unordered_map<const decl*, llvm::Value*>;

struct cg_context {
    cg_context() : ll(new llvm:LLVMContext()) {}
    
    ~cg_context() {
        delete ll;
    }

llvm:LLVMContext *getContext() const {
     return ll;
     }


     std::string getName(const decl* d);


     llvm::Type* getType(const type* t);
     llvm::Type* getBoolType(const boolType* t);
     llvm::Type* getCharType(const char_type* t);
     llvm::Type* getIntType(const int_type* t);
     llvm::Type* getFloatType(const float_type* t);
     llvm::Type* getRefType(const ref_type* t);
     llvm::Type* getFnType(const fn_type* t);


     llvm::Type* getType(const typed_decl* d);


     llvm::LLVMContext* ll;

};


struct cg_module {
    cg_module(cg_context* context, const prog_decl* prog);

    llvm::LLVMContext* getContext() const  {
      return parent->getContext();
    }

    llvm::Module* getModule() const {
        return mod;
    }

    std::string getName(const decl* d) {
        return parent->geetName(d);
    }

    llvm::Type* getType(const type* t) {
        return parent->getType(t);
    }

    llvm::Type* getType(const typed_decl* d) {
        return parent->getType(d);
    }

    void declare(const decl* d, llvm::GLobalValue* v);


    llvm::GlobalValue* lookup(const decl* d) const;

    void generate();
    void generate(const decl* d);
    void generateVarDecl(Const var_decl* d);
    void generateFnDecl(const fn_decl* d);

    cg_context* parent;

    const prog_decl* prog;

    llvm::Module* mod;

    variable_map globals;
};


struct cg_function {
    cg_function(cg_module& m, const fn_decl* d);

    llvm::LLVMContext* getContext() const {
      return parent->getContext();
    }

    llvm::Module* getModule() const {
        return parent->getModule();
    }

    llvm::Function* getFunction() const {
        return fn;
    }


    std::string getName(const decl* d) {
        return parent->getName(d);
    }


    llvm::Type* getType(const type* t) {
        return parent->getType(t);
    }

    llvm::Type* getType(const expr* e) {
        return getType(e->getType());
    }

    llvm::Type* getType(const typed_decl* t) {
        return parent->getType(t);
    }


    void declare(const decl* x, llvm::Value* v);
    
    llvm::Value* lookup(const decl* x) const;

    void define();

    llvm::BasicBlock* getEntryBlock() const {
        return entry;
    }

    llvm::BasiBlock* getCurrentBlock() const {
        return curr;
    }

    llvm::BasicBlock* makeBlock(const char* label);


    void emitBlock(llvm::BasicBlock* bb);


    llvm::Value* generateExpr(const expr* e);
    llvm::Value* generateBoolExpr(const bool_expr* e);
    llvm::Value* generateIntExpr(const int_expr* e);
    llvm::Value* generateFloatExpr(const float_expr* e);
    llvm::Value* generateIdExpr(const id_expr* e);

    llvm::Value* generateUnopExpr(const unop_expr* e);
    llvm::Value* generateArithmeticExpr(const unop_expr* e);
    llvm::Value* generateIntExpr(const unop_expr* e);
    llvm::Value* generateFloatExpr(const unop_expr* e);
    llvm::Value* generateBitwiseExpr(const unop_expr* e);
    llvm::Value* generateLogicalExpr(const unop_expr* e);
    llvm::Value* generateAddressExpr(const unop_expr* e);
    llvm::Value* generateDerefExpr(const unop_expr* e);

    llvm::Value* generateBinopExpr(const binop_expr* e);
    llvm::Value* generateArithmeticExpr(const binop_expr* e);
    llvm::Value* genetateIntExpr(const binop_expr* e);
    llvm::Value* generateFloatExpr(const binop_expr* e);
    llvm::Value* generateBitwiseExpr(const binop_expr* e);
    llvm::Value* generateLogicalExpr(const binop_expr* e);
    llvm::Value* generateAndExpr(const binop_expr* e);
    llvm::Value* generateOrExpr(const binop_expr* e);
    llvm::Value* generateRelationalExpr(const binop_expr* e);

    llvm::Value* generateCallExpr(const call_expr* e);
    llvm::value* generateIndexExpr(const index_expr* e);
    llvm::Value* generateCondExpr(const cond_expr* e);
    llvm::Value* generateAssignExpr(const assign_expr* e);
    llvm::Value* generateConvExpr(const conv_expr* e);


    void generateStmt(const stmt* s);
    void generateBlockStmt(const block_stmt* s);
    void generateWhenStmt(const when_stmt* s);
    void generateIfStmt(const if_stmt* s);
    void generateWhileStmt(const while_stmt* s);
    void generarteBreakStmt(const break_stmt* s);
    void generateContStmt(const cont_stmt* s);
    void generateRetStmt(const ret_stmt* s);
    void generateDeclStmt(const decl_stmt* s);
    void generateExprStmt(const expr_stmt* s);

    void generateDecl(const decl* d);
    void generateVarDecl(const var_decl* d);


    void makeVariable(const var_decl* d);
    void makeReference(const var_decl* d);


    cg_module* parent;

    const fn_decl* src;

    llvm::Function* fn;

    llvm::BasicBlock* entry;

    llvm::BasicBlock* curr;

    variable_map locals;
};

std::string cg_context::getType(const type* t) {

  switch (t->getKind()) {
    case type::bool_kind:
      return getBoolType(static_cast<const bool_type*>(t));

    case type::char_kind:
      return getCharType(static_cast<const char_type*>(t));
    case type::int_kind:
      return getIntType(static_cast<const int_type*>(t));
    case type::float_kind:
      return getFloatType(static_cast<const float_type*>(t));
    case ref_kind:
      return getRefType(static_cast<const ref_type*>(t));
    case type::fn_kind:
      return getFnType(static_cast<const fn_type*>(t));
    default:
      throw std::logic_error("Not a valid type");
  }
}

llvm::Type* cg_context::getBoolType(const bool_type* t) {
  return llvm::Type::getInt1Ty(*ll);
}

llvm::Type* cg_context::getCharType(const char_type* t) {
    return llvm::Type::getInt8Ty(*ll);
}

llvm::Type* cg_context::getIntType(const int_type* t) {
    return llvm::Type::getInt32Ty(*ll);
}

llvm::Type* cg_context::getFloatType(const float_type* t) {
    return llvm::Type::getFloatTy(*ll);
}

llvm::Type* cg_context::getRefTyppe(const ref_type* t) {
    llvm::Type* obj = getType(->getObjectType());
    return obj->getPointerTo();
}


llvm::Type* cg_contexxt::getFnType(const fn_type* t) {
    const type_list& ps = t->getParameterTypes();
    std::vector<llvm::Type*> parms(ps.size());
    std::transform(ps.begin(), ps.end(), parms,begin(), [this](const type* p) {
        return getType(p);
        });
    llvm::Type* ret = getType(t->getReturnType());
    llvm::Type* base = llvm::FunctionType::get(ret, parms, false);
    return base->getPointerTo();
}

llvm::Type* cg_context::getType(const typed_decl* d) {
    return getType(d->getType());
}


cg_module::cg_module(cg_context& cxt, const prog_decl8 prog) : parent(&cxt), prog(prog), mod(new llvm::Module("a.ll", *getContext())) {}

void cg_module::declare(const decl* d, llvm::GlobalValue* v) {
    assert(globals.count(d) == 0);
    globals.exmplace(d, v);
}


llvm::GlobalValue* cg_module::lookup(const decl* d) const {
    auto iter = globals.find(d);
    if (iter != globals.end()) {
        return llvm::cast<llvm::GLobalValue>(iter->second);
    } else {
        return nullptr;
    }
}


void cg_module::generate() {
    for (const decl* d : prog->getDeclarations()) {
        generate(d);
    }
}


void cg_module:;generate(const decl* d) {
    switch(d->getKind()) {
        case decl::var_kind:
          return generateVarDecl(static_cast<const var_decl*>(d));
        case decl::fn_kind:
          return generateFnDecl(static_cast<const fn_decl*>(d));
        default:
          throw std::logic_error("Not a valid declaration");
    }
}

void cg_module::generateVarDecl(const var_decl* d) {
    std::string n = getName(d);
    llvm::Type* t = getType(d->getType());
    llvm::Constant* c = llvm::Constant::getNullValuet(t);
    llvm::GLobalVariable* var = new llvm::GlobalVariable( *mod, t, false, llvm::GlobalVariable::ExternalLinkage, c, n);

    declare(d, var);
}


void cg_module::generateFnDecl(const fn_decl* d) {
    cg_function fn(*this, d);
    fn.define();
}


static llvm::FunctionType* getFnType(llvm::Type* t) {
    assert(llvm::isa<<llvm::PointerType>(t));
    return llvm::cast<llvm::FunctionType>(t->getPointerElementType());
}


cg_function::cg_function(cg_module& m, const fn_decl* d) : parent(&m), src(d), fn(), entry(), curr() {
    std::string n = getName(d);
    llvm::Type* t = getType(d);
    fn = llvm::Function::Create(getFnType(t), llvm::Function:;ExternalLinkage, n, getModule());


    parent->declare(d, fn);


    entry = makeBlock("entry");
    emitBlock(entry);

    llvm::IRBuilder<> ir(getCurrentBlock());

    assert(d->getParameters().size() == fn->arg_size());
    auto pi = d->getParameters().begin();
    auto ai = fn->arg_begin();

    while (ai != fn->arg_end()) {
        const parm_decl* parm = staic_cast<const parm_decl*>(*pi);
        llvm::Argument& arg = *ai;

        arg.setName(getName(parm));

llvm:Value* var = ir.CreateAlloca(arg.egtType(), nullptr, arg.egtName());
     decalre(parm, var);

     ir.CreateStore(&arg, var);
     ++ai;
     ++pi;
    }

}

void cg_function::declare(const decl* d, llvm::Value* v) {
    assert(locals.count(d) == 0);
    locals.emplace(d, v);
}


llvm::Value* cg_function::lookup(const decl* d) const {
    auto iter = locals.find(d);
    if (iter != locals.end()) {
        return iter->second;
    } else {
        return parent->lookup(d);
    }
}

llvm::BasicBlock* cg_function::make_block(const char* label) {
    return llvm::BasicBlock::Create(*get_context(), label);
}

void cg_function::emit_block(llvm::BasicBlock* bb) {
    bb->insertInto(getFunction());
    curr = bb;
}

void cg_function::define() {
    generateStmt(src->getBody());
}


//Determine which base xpression type it is, and go from there

llvm::Value* cg_function::generateExpr(const expr* e) {
    switch (e->getKind()) {
      case expr::bool_kind:
        return generateBoolExpr(static_cast<const bool_expr*>(e));
      case expr::int_kind:
        return generateIntExpr(static_cast<const int_expr*>(e));
      case expr::float_kind:
        return generateFloatExpr(static_cast<const float_expr*>(e));
      case expr::id_kind:
        return generateIdExpr(static_cast<const id_expr*>(e));
      case expr::unop_kind:
        return generateUnopExpr(static_cast<const unop_expr*>(e));
      case expr::binop_kind:
        return generateBinopExpr(static_cast<const binop_expr*>(e));
      case expr::call_kind:
        return generateCallExpr(static_cast<const call_expr*>(e));
      case expr::index_kind:
        return generateIndexExpr(static_cast<const index_expr*>(e));
      case expr::cond_kind:
        return generateCondExpr(static_cast<const cond_expr*>(e));
      case expr::assign_kind:
        return generateAssignExpr(static_cast<const assign_expr*>(e));
      case expr::conv_kind:
        return genreateConvExpr(static_cast<const conv_expr*>(e));
      default:
        throw std::runtime_error("not avalid expression");
    }
}

llvm::Value* cg_function::generateBoolExpr(const bool_expr* e) {
    return llvm::ConstantInt::get(getType(e), e->getValue(), false);
}

llvm::Value* cg_function::generateIntExpr(const int_expr* e) {
    return llvm::ConstantInt::get(getType(e), e->getValue(), true);
}

llvm::Value* cg_function::generateFloatExpr(const float_expr* e) {
    return llvm::ConstantFP::get(getType(e), e->getValue());
}

llvm::Value* cg_function::generateIdExpr(const id_expr* e) {
  decl* ref = e->ref;
  switch (ref->getKind()) {
    case const_kind:
        return llvm::ConstantExpr::get(getType(ref), e->getInit());
    case var_kind:
      return generateVarDecl(static_cast<const var_decl*>(e));
    case fn_kind:
      return generateFnDecl(static_cast<const fn_decl*>(e));
    default:
      throw std::runtime_error("not a valid ID EXPR");
  }
}

llvm::Value* cg_function::generateUnopExpr(const unop_expr* e) {
  unop* op = unop_expr->m_op;
  expr* exp = unop_expr->m_arg;
  llvm::Constant cons = 
  switch (op) {
    case uo_pos:
        //The + unary op does not have any effect by itself, let it pass.
        return cg_function::generateExpr(exp);
    case uo_neg:
        //Should function like 0 - expr
        expr* zeroExpr = new int_expr(exp->getType(), 0);
        expr* binopExpr = new binop_expr(exp->getType(), bo_sub, exp, zeroExpr);
        return cg_function::generateBinopExpr(binopExpr);
    case uo_not:
    case uo_cmp:
    case uo_addr:
    case uo_deref:
        return generateDerefExpr(e);
        return 
  }
}

llvm::Value* cg_function::generateDerefExpr(const unop_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateBinopExpr(const binop_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateRelationalExpr(const binop_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateCallExpr(const call_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateIndexExpr(const index_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateAssignExpr(const assign_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateCondExpr(const cond_expr* e) {
    return nullptr;
}

llvm::Value* cg_function::generateConvExpr(const conv_expr* c) {
    return nullptr;
}

void cg_function::generateStmt(const stmt* s) {
    switch (s->getKind()) {
    case stmt::block_kind:
      return generateBlockStmt(static_cast<const block_stmt*>(s));
    case stmt::when_kind:
      return generateWhenStmt(static_cast<const when_stmt*>(s));
    case stmt::if_kind:
      return generateIfStmt(static_cast<const if_stmt*>(s));
    case stmt::while_kind:
      return generateWhileStmt(static_cat<const while_stmt*>(s));
    case stmt::break_kind:
      return generateBreakStmt(static_cast<const break_stmt*>(s));
    case stmt::cont_kind:
      return generateContStmt(static_cast<const cont_stmt*>(s));
    case stmt::ret_kind:
      return generateRetStmt(static_cast<const ret_stmt*>(s));
    case stmt::decl_kind:
      return generateDeclStmt(static_cast<const decl_stmt*>(s));
    case stmt::expr_kind:
      return generateExprStmt(static_cast<const expr_stmt*>(s));
    }
}

void cg_function::generateBlockStmt(const block_stmt* s) {

}

void cg_function::generateWhenStmt(const when_stmt* s) {

}

void cg_function::generateIfStmt(const if_stmt* s) {

}

void cg_function::generateWhileStmt(const while_stmt* s) {

}

void cg_function::generateBreakStmt(const break_stmt* e) {

}

void cg_function::generateContStmt(const cont_stmt* e) {

}

void cg_function::generateRetStmt(const ret_stmt* e) {

}

void cg_function::generateDeclStmt(const decl_stmt* e) {

}


void cg_function::generateExprStmt(const expr_stmt* e) {

}

void generate(const decl* d) {
    assert(d->isProgram());

    cg_context cg;

    cg_module mod(cg, static_cast<const prog_decl*>(d));
    mod.generate();
    
    llvm::outs() << *mod.getModule();
}
