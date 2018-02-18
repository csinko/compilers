#pragma once

#include <iosfwd>

class file;

class location {
  public:
    location() : source(nullptr), line(-1), column(-1) {}

    location(const file& f, int l, int c) : source(&f), line(l), column(c) {}

    explicit operator bool() const {
      return line >= 0;
    }

    const file* source;
    int line;
    int column;
};

std::ostream& operator<<(std::ostream& os, location loc);
