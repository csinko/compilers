#include "expr.hpp"
#include "type.hpp"


type* expr::getObjectType() const {
return  m_type->getObjectType();
}

bool expr::hasType(const type* t) const {
return  m_type->isBool();
}

bool expr::isBool() const {
    return m_type->isBool();
}

bool expr::isInt() const {
    return m_type->isInt();
}

bool expr::isFunction() const {
    return m_type->isFunction();
}

bool expr::isArithmetic() const {
    return m_type->isArithmetic();
}

bool expr::isNumeric() const {
    return m_type->isNumeric();
}

bool expr::isScalar() const {
    return m_type->isScalar();
}
