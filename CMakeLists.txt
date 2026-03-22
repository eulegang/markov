cmake_minimum_required(VERSION 4.2)

project(markov
  LANGUAGES CXX
  VERSION 1.0.0
  DESCRIPTION "markov chains"
)

add_library(
  markov SHARED 

  src/distribution.cpp
  src/distribution_weights.cpp
  src/transition.cpp
  src/transitions_weights.cpp
)

add_library(
  markov_static STATIC 

  src/distribution.cpp
  src/distribution_weights.cpp
  src/transition.cpp
  src/transitions_weights.cpp
)

set_property(TARGET markov_static PROPERTY POSITION_INDEPENDENT_CODE ON)


include(cmake/gdb.cmake)
include(cmake/test.cmake)
add_subdirectory(node)

