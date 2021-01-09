cmake_minimum_required (VERSION 3.9)
project (readsparse VERSION 0.1)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)

set(SRC_FILES ${PROJECT_SOURCE_DIR}/src/reader.cpp
              ${PROJECT_SOURCE_DIR}/src/writer.cpp)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(BUILD_SHARED_LIBS True)
add_library(readsparse SHARED ${SRC_FILES})

if (WIN32 OR MSVC)
    add_compile_definitions(_FILE_OFFSET_BITS=64)
endif()

if (MSVC)
    add_compile_options(/O2)
else()
    add_compile_options(-O2 -march=native)
endif()

set(HEADER_FILES ${PROJECT_SOURCE_DIR}/src/readsparse.hpp
                 ${PROJECT_SOURCE_DIR}/src/readsparse_detemplated.hpp)
set_target_properties(readsparse PROPERTIES PUBLIC_HEADER HEADER_FILES)

# Install target
include(GNUInstallDirs)

install(TARGETS readsparse
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(readsparse.pc.in readsparse.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/readsparse.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
