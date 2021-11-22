cmake_minimum_required(VERSION 3.16)

project(CIL
       VERSION 0.1.0
       LANGUAGES C CXX
       DESCRIPTION "C++ Image Library")

find_package(PkgConfig REQUIRED)

option(LIBPNG_DIR "Path to libpng installation directory")

add_subdirectory(lib)

install(DIRECTORY include/ TYPE INCLUDE)