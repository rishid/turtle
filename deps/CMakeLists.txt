include(ExternalProject)
ExternalProject_Add(libffi
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libffi
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libffi/./autogen.sh COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libffi/configure --prefix=<INSTALL_DIR>
    BUILD_COMMAND ${MAKE}
    # Disable install step
    #INSTALL_COMMAND ""
    BUILD_IN_SOURCE 1
    )

# Cannot use find_package with an ExternalProject in CMake, so for now set these variables manually
ExternalProject_Get_Property(libffi install_dir)
set(LIBFFI_LIBRARIES "ffi" CACHE INTERNAL "")
set(LIBFFI_INCLUDE_DIRS "${install_dir}/lib/libffi-3.99999/include" CACHE INTERNAL "")
set(LIBFFI_LIBRARY_DIRS "${install_dir}/lib/" CACHE INTERNAL "")

# build libart as a library
add_library(art libart/src/art.c)
set(LIBART_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/libart/src" CACHE INTERNAL "")

