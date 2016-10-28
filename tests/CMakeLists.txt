
link_directories("${LIBFFI_LIBRARY_DIRS}")

include_directories(${PROJECT_SOURCE_DIR}/src "${LIBFFI_INCLUDE_DIRS}")

add_executable(ffi_sanity ffi_sanity.c)
target_link_libraries(ffi_sanity ${LIBFFI_LIBRARIES} art)
add_test(NAME SanityCheck COMMAND ffi_sanity)

add_executable(turtle_example turtle_example.c)
target_link_libraries(turtle_example turtle ${LIBFFI_LIBRARIES} art)
set_target_properties(turtle_example PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")
add_test(NAME TurtleExample COMMAND turtle_example)

add_executable(turtle_test turtle_test.c)
target_link_libraries(turtle_test turtle ${LIBFFI_LIBRARIES} art)
set_target_properties(turtle_test PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")
add_test(NAME TurtleTest COMMAND turtle_test)
