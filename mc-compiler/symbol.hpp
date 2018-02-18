#pragma once

#include <string>
#include<unordered_set>
// A symbol is...
using symbol = const std::string*;

//A sumbol table is a collection of symbol.s  These are used to ensure that sting comparison is *fast*.
//Many applications link additional information to symbols.  Sutton does not#p.

class symbol_table {
  public:
    symbol get(const char* str);
    symbol get(const std::string& str);
  private:
    std::unordered_set<std::string> m_syms;
};

inline symbol symbol_table::get(const char* str) {
  return &*m_syms.insert(str).first;
}


//Returns a unique symbol for the spelling of 'str'.
inline symbol symbol_table::get(const std::string& str) {
  return &*m_syms.insert(str).first;
}
