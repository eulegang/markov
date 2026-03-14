cmake_minimum_required(VERSION 4.2)

project(markov
  LANGUAGES CXX
  VERSION 1.0.0
  DESCRIPTION "markov chains"
)


file(GLOB LIB_SRC "src/*.cpp")
add_library(markov SHARED ${LIB_SRC})

include(cmake/gdb.cmake)
include(cmake/test.cmake)

