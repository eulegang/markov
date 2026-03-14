find_package(GTest REQUIRED)

enable_testing()
file(GLOB TEST_SRC "tests/*.cpp")
add_executable(unit ${TEST_SRC})

target_include_directories(unit PUBLIC ./src)
target_link_libraries(unit markov)
target_link_libraries(unit GTest::gtest_main)


add_test(unit unit)
