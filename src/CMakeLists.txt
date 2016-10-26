    
# Specify include dir for googlemock and googletest
#ExternalProject_Get_Property(googlemock source_dir)
#include_directories(${source_dir}/include)
#include_directories(${source_dir}/gtest/include)

# Create dependency of MainTest on googlemock
#add_dependencies(MainTest googlemock)

# Specify MainTest's link libraries
#ExternalProject_Get_Property(libffi install_dir)
#target_link_libraries(MainTest
#                      debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES}
#                      optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_FIND_LIBRARY_SUFFIXES})
    
#target_link_libraries(testapp ${SDL2_LIBRARIES})
#target_include_directories(testapp PUBLIC ${SDL2_INCLUDE_DIRS})

#set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${install_dir}/lib/pkgconfig" )
#find_package(PkgConfig REQUIRED)
#pkg_search_module(LIBFFI REQUIRED libffi)

include_directories("${PROJECT_BINARY_DIR}")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}"
#  "${CMAKE_CURRENT_SOURCE_DIR}/libffi/x86")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}"
  "${LIBFFI_INCLUDE_DIRS}"
  "${LIBART_INCLUDE_DIRS}")

include_directories(${INCLUDES})
    
add_sources(turtle.c parser.c)
add_library(turtle ${SRCS})
add_dependencies(turtle libffi art)
#target_link_libraries(turtle libffi art)

#install(TARGETS turtle
#  DESTINATION ${OUTPUT_LIB_DIR}
#  )
#install(FILES ${PUBLIC_HEADERS}
#  PERMISSIONS OWNER_READ GROUP_READ
#  DESTINATION ${OUTPUT_INC_DIR}
#  )

