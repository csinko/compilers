
set (CMAKE_CX_FLAGS " -std=c++lz")

add_library(mc
    file.cpp
    location.cpp
    symbol.cpp
    token.cpp
    lexer.cpp
    parser.cpp
    semantics.cpp
    scope.cpp
    type.cpp
    expr.cpp
    decl.cpp
    stmt.cpp)

