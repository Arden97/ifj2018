project(ifj18)
include_directories( ${MY_SOURCE_DIR} )
set(CMAKE_C_FLAGS "-std=c99 -lm")
cmake_minimum_required(VERSION 3.12)
add_executable (ifj18 error.c khash.h  semantics.c semantics.h error.h gc.h list.h prettyprint.h stack.h strlib.h symtable.h utils.h expression.c gc.c list.c parser.c prettyprint.c scanner.c stack.c strlib.c symtable.c utils.c ifj18.c)
target_link_libraries(ifj18 m)