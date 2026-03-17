
file(GLOB NODEJS_SRC "src/nodejs/*.cpp")
add_library(nodejs SHARED ${NODEJS_SRC})
set_target_properties(
  nodejs PROPERTIES 
  OUTPUT_NAME "markov" 
  SUFFIX ".node"
  PREFIX ""
)

target_link_libraries(nodejs markov)
target_include_directories(nodejs PUBLIC ./src)

