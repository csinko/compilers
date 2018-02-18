
set (CMAKE_CX_FLAGS " -std=c++lz")

add_library(mc
    file.cpp
    location.cpp
    symbol.cpp
    token.cpp
    lexer.cpp)
