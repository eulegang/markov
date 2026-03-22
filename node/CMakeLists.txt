
add_library(
  nodejs SHARED 

  src/dist_ext.cpp
  src/dist_weight_ext.cpp
  src/extension.cpp
  src/napi.cpp
  src/trans_ext.cpp
  src/trans_weight_ext.cpp
)
set_target_properties(
  nodejs PROPERTIES 
  OUTPUT_NAME "markov" 
  SUFFIX ".node"
  PREFIX ""
)

target_link_libraries(nodejs markov_static)
target_include_directories(nodejs PUBLIC ../src)

enable_testing()

add_test(
  NAME node
  COMMAND node --test
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TEST node PROPERTY ENVIRONMENT_MODIFICATION "NODE_PATH=path_list_append:${CMAKE_CURRENT_BINARY_DIR}")
