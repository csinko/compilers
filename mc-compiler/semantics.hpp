#pragma once

#include "token.hpp"
#include <vector>

class type;
class expr;
class stmt;
class decl;
class fn_decl;

using type_list = std::vector<type*>;
using expr_list = std::vector<expr*>;
using stmt_list = std::vector<stmt*>;
using decl_list = std::vector<decl*>;

class scope;

class semantics {
  public:
    semantics();
    ~semantics();

    type* onBasicType(token tok);

    expr* onAssignmentExpression(expr* e1, expr* e2);
    expr* onConditionalExpression(expr* e1, expr* e2, expr* e3);
    expr* onLogicalOrExpression(expr* e1, expr* e2);
    expr* onLogicalAndExpression(expr* e1, expr* e2);
    expr* onBitwiseOrExpression(expr* e1, expr* e2);
    expr* onBitwiseXorExpression(expr* e1, expr* e2);
    expr* onBitwiseAndExpression(expr* e1, expr* e2);
    expr* onEqualityExpression(token tok, expr* e1, expr* e2);
    expr* onRelationalExpression(token tok, expr* e1, expr* e2);
    expr* onShiftExpression(token tok, expr* e1, expr* e2);
    expr* onAdditiveExpression(token tok, expr* e1, expr* e2);
    expr* onMultiplicativeExpression(token tok, expr* e1, expr* e2);
    expr* onCastExpression(expr* e, type* t);
    expr* onUnaryExpression(token tok, expr* e);
    expr* onCallExpression(expr* e, const expr_list& args);
    expr* onIndexExpression(expr* e, const expr_list& args);
    expr* onIdExpression(token tok);
    expr* onIntegerLiteral(token tok);
    expr* onBooleanLiteral(token tok);
    expr* onFloatLiteral(token tok);

    stmt* onBlockStatement(const stmt_list& ss);
    void startBlock();
    void finishBlock();
    stmt* onIfStatement(expr* e, stmt* s1, stmt* s2);
    stmt* onWhileStatement(expr* e, stmt* s);
    stmt* onBreakStatement();
    stmt* onContinueStatement();
    stmt* onReturnStatement(expr* e);
    stmt* onDeclarationStatement(decl* d);
    stmt* onExpressionStatement(expr* e);

    decl* onVariableDeclaration(token n, type* t);
    decl* onVariableDefinition(decl*, expr* e);
    decl* onConstantDeclaration(token n, type* t);
    decl* onConstantDefinition(decl*, expr* e);
    decl* onValueDeclaration(token n, type* t);
    decl* onValueDefinition(decl*, expr* e);
    decl* onParameterDeclaration(token n, type* t);
    decl* onFunctionDeclaration(token n, const decl_list& parms, type* ret);
    decl* onFunctionDefinition(decl* d, stmt* s);

    decl* onProgram(const decl_list& dl);

    void enterGlobalScope();
    void enterParameterScope();
    void enterBlockScope();
    void leaveScope();
    scope* getCurrentScope() const {
      return m_scope;
    }

    fn_decl* getCurrentFunction() const {
      return m_fn;
    }

    void declare(decl* d);

    decl* lookup(symbol n);

    expr* requireReference(expr* e);
    expr* requireValue(expr* e);
    expr* requireInteger(expr* e);
    expr* requireBoolean(expr* e);
    expr* requireFunction(expr* e);
    expr* requireArithmetic(expr* e);
    expr* requireNumeric(expr* e);
    expr*requireScalar(expr* e);

    type* requireSame(type* t1, type* t2);
    type* commonType(type* t1, type* t2);

    expr* convertToValue(expr* e);
    expr* convertToBool(expr* e);
    expr* convertToChar(expr* e);
    expr* convertToInt(expr* e);
    expr* convertToFloat(expr* e);
    expr* convertToType(expr* e, type* t);

  private:
    scope* m_scope;

    fn_decl* m_fn;

    type* m_bool;
    type* m_char;
    type* m_int;
    type* m_float;
};
