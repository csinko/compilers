#include "location.hpp"
#include "file.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, location loc) {
  if (loc.source) {
    os << loc.source->getPath();
  }
  else {
    os << "<input>";
  }
    os << ':' << loc.line + 1 << ':' << loc.column + 1;
    return os;
}
