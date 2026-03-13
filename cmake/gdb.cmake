if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  find_package(Python3 COMPONENTS Interpreter REQUIRED)

  add_custom_command(
    TARGET markov
    POST_BUILD
    COMMAND ${Python3_EXECUTABLE}
    ARGS ${CMAKE_SOURCE_DIR}/cmake/add_autoload.py ${CMAKE_SOURCE_DIR}/gdb/autoload.py ${CMAKE_OBJCOPY} $<TARGET_FILE:markov>
    # --add-section .debug_gdb_scripts=${CMAKE_SOURCE_DIR}/gdb/autoload.py $<TARGET_FILE:markov>
    COMMENT "inserting gdb pretty printers"
  )
endif()

