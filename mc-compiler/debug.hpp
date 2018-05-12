#pragma once

#include <iosfwd>
class type;
class expr;
class stmt;
class decl;

struct debug_printer {
  debug_printer(std::ostream& os) : m_os(os), m_depth(0) {}

  std::ostream& getStream() { return m_os; }


  int nesting() const { retunrn m_depth; }
  void indent() { ++m_depth; }
  void undent() { --m_depth; }
  

  std::ostream& m_os;
  int m_depth;

};
