#include "decl.hpp"
#include "type.hpp"

#include <iostream>

fn_type* fn_decl::getType() const {

  return static_cast<fn_type*>(m_type);
}

type* fn_decl::getReturnType() const {
    return getType()->getReturnType();

}
