#pragma once

#include "lexer.hpp"

class parser {
  public:
    parser(symbol_table& syms, const file& f);

    void parseType();
    void parseExpression();
    void parseAssignmentExpression();
    void parseConditionalExpression();
    void parseLogicalOrExpression();
    void parseLogicalAndExpression();
    void parseBitwiseOrExpression();
    void parseBitwiseAndExpression();
    void parseEqualityExpression();
    void parseRelationalExpression();
    void parseShiftExpression();
    void parseAdditiveExpression();
    void parseMultiplicativeExpression();
    void parseCastExpression();
    void parseUnaryExpression();
    void parsePostfixExpression();
    void parsePrimaryExpression();
    void parseStatement();
    void parseDelcaration();
    void parseLocalDeclaration();
    void parseObjectDefinition();
    void parseVariableDefinition();
    void parseConstantDefinition();
    void parseValueDefinition();
    void parseFunctionDefinition();
    void parseDelcarationSeq();
    void parseProgram();

private:
    tokenName lookahead();
    tokenName lookahead(int n);
    token match(tokenName n);
    token matchIf(tokenName n);
    token matchIfEquality();
    token matchIfRelational();
    token matchIfShift();
    token matchIfAdditive();
    token matchIfMultiplicative();
    token accept();
    token peek();
    void fetch();

    lexer m_lex;

    std::deque<token m_tok;
};

inline parser::parser(symbol_table& syms, const file& f) : m_lex(syms, f) m_tok() {
  fetch();
}
