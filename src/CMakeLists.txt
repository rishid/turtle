    
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}"
  "${LIBFFI_INCLUDE_DIRS}"
  "${LIBART_INCLUDE_DIRS}")

include_directories(${INCLUDES})
    
add_sources(turtle.c parser.c)
add_library(turtle ${SRCS})
add_dependencies(turtle libffi art)
#target_link_libraries(turtle libffi art)

