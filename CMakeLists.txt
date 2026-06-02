cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(stv CXX C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(MPV REQUIRED IMPORTED_TARGET mpv)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)

add_executable(stv src/main.cpp)

target_link_libraries(stv PRIVATE PkgConfig::MPV)
target_link_libraries(stv PRIVATE PkgConfig::SDL3)
