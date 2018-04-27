#pragma once

#include "lexer.hpp"
#include "semantics.hpp"


#include <deque>
#include <vector>

class type;
class expr;
class stmt;
class decl;
class prog;

using type_list = std::vector<type*>;
using expr_list = std::vector<expr*>;
using stmt_list = std::vector<stmt*>;
using decl_list = std::vector<decl*>;

class parser {
  public:
    parser(symbol_table& syms, const file& f);

    //Types
    type* parseType();
    type* parseBasicType();

    //Expressions
    expr* parseExpression();
    expr* parseAssignmentExpression();
    expr* parseConditionalExpression();
    expr* parseLogicalOrExpression();
    expr* parseLogicalAndExpression();
    expr* parseBitwiseOrExpression();
    expr* parseBitwiseAndExpression();
    expr* parseBitwiseXorExpression();
    expr* parseEqualityExpression();
    expr* parseRelationalExpression();
    expr* parseShiftExpression();
    expr* parseAdditiveExpression();
    expr* parseMultiplicativeExpression();
    expr* parseCastExpression();
    expr* parseUnaryExpression();
    expr* parsePostfixExpression();
    expr* parsePrimaryExpression();

    expr_list parseArgumentList();
    
    //Statements
    stmt* parseStatement();
    stmt* parseBlockStatement();
    stmt* parseIfStatement();
    stmt* parseWhileStatement();
    stmt* parseBreakStatement();
    stmt* parseContinueStatement();
    stmt* parseReturnStatement();
    stmt* parseDeclarationStatement();
    stmt* parseExpressionStatement();
    
    stmt_list parseStatementSeq();

    //Delclarations
     decl* parseDeclaration();
     decl* parseLocalDeclaration();
     decl* parseObjectDefinition();
     decl* parseVariableDefinition();
     decl* parseConstantDefinition();
     decl* parseValueDefinition();
     decl* parseFunctionDefinition();
     decl* parseParameter();
     decl* parseProgram();

     decl_list parseDeclarationSeq();
     decl_list parseParameterClause();
     decl_list parseParameterList();

private:
    token_name lookahead();
    token_name lookahead(int n);
    token match(token_name n);
    token matchIf(token_name n);
    token matchIfEquality();
    token matchIfRelational();
    token matchIfShift();
    token matchIfAdditive();
    token matchIfMultiplicative();
    token accept();
    token peek();
    token matchIfLogicalOr();
    token matchIfLogicalAnd();
    token matchIfBitwiseOr();
    token matchIfBitwiseXor();
    token matchIfBitwiseAnd();
    void fetch();

    lexer m_lex;
    semantics m_act;

    std::deque<token> m_tok;
};

inline parser::parser(symbol_table& syms, const file& f) : m_lex(syms, f), m_tok() {
  fetch();
}
