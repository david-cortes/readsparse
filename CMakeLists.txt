cmake_minimum_required (VERSION 3.12.4)
project (readsparse VERSION 0.1.5)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)

set(SRC_FILES ${PROJECT_SOURCE_DIR}/src/reader.cpp
              ${PROJECT_SOURCE_DIR}/src/writer.cpp)
set(BUILD_SHARED_LIBS True)
add_library(readsparse SHARED ${SRC_FILES})
add_compile_definitions(READSPARSE_COMPILE)

if (WIN32 OR MSVC)
    add_compile_definitions(_FILE_OFFSET_BITS=64)
endif()

# Compiler optimizations
include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
    "
    int main(int argc, char **argv)
    {
        double *__restrict x = nullptr;
        return 0;
    }
    "
    SUPPORTS_RESTRICT
)
if (SUPPORTS_RESTRICT)
    add_compile_definitions(SUPPORTS_RESTRICT=1)
endif()

if (MSVC)
    if (NOT (${CMAKE_CXX_FLAGS_RELEASE} MATCHES "/O2"))
        set(OLD_FLAGS ${CMAKE_REQUIRED_FLAGS})
        set(CMAKE_REQUIRED_FLAGS "/O2")
        check_cxx_source_compiles(
            "
            int main(int argc, char **argv)
            {
                return 0;
            }
            "
            SUPPORTS_O2
        )
        set(CMAKE_REQUIRED_FLAGS ${OLD_FLAGS})
        if (SUPPORTS_O2)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
        endif()
    endif()
else()
    if (NOT (${CMAKE_CXX_FLAGS_RELEASE} MATCHES "-O3 "))
        set(OLD_FLAGS ${CMAKE_REQUIRED_FLAGS})
        set(CMAKE_REQUIRED_FLAGS "-O3")
        check_cxx_source_compiles(
            "
            int main(int argc, char **argv)
            {
                return 0;
            }
            "
            SUPPORTS_O3
        )
        set(CMAKE_REQUIRED_FLAGS ${OLD_FLAGS})
        if (SUPPORTS_O3)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        else()
            set(CMAKE_REQUIRED_FLAGS "-O2")
            check_cxx_source_compiles(
                "
                int main(int argc, char **argv)
                {
                    return 0;
                }
                "
                SUPPORTS_O2
            )
            set(CMAKE_REQUIRED_FLAGS ${OLD_FLAGS})
            if (SUPPORTS_O2)
                set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
            endif()
        endif()
    endif()
endif()

option(USE_MARCH_NATIVE "Build with -march=native" OFF)
if (USE_MARCH_NATIVE AND NOT MSVC)
    set(OLD_FLAGS ${CMAKE_REQUIRED_FLAGS})
    set(CMAKE_REQUIRED_FLAGS "-march=native")
    check_cxx_source_compiles(
        "
        int main(int argc, char **argv)
        {
            return 0;
        }
        "
        SUPPORTS_MARCH_NATIVE
    )
    set(CMAKE_REQUIRED_FLAGS ${OLD_FLAGS})
    if (SUPPORTS_MARCH_NATIVE)
        message(STATUS "Adding flag -march=native.")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    else()
        set(CMAKE_REQUIRED_FLAGS "-mcpu=native")
        check_cxx_source_compiles(
            "
            int main(int argc, char **argv)
            {
                return 0;
            }
            "
            SUPPORTS_MCPU_NATIVE
        )
        set(CMAKE_REQUIRED_FLAGS ${OLD_FLAGS})
        if (SUPPORTS_MCPU_NATIVE)
            message(STATUS "Adding flag -mcpu=native.")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=native")
        else()
            message(WARNING "Flags -march=native and -mcpu=native not supported by the compiler.")
        endif()
    endif()
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
