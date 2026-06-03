cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OUTPUT_LIB_DIR "${CMAKE_BINARY_DIR}/lib")

project(stv CXX C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(MPV REQUIRED IMPORTED_TARGET mpv)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SQLite3 REQUIRED IMPORTED_TARGET sqlite3)

add_executable(stv src/main.cpp src/db.cpp)
add_executable(stv_migration src/migrate.cpp src/db.cpp)
add_library(std OBJECT carbon-src/std/print.cpp)

target_link_libraries(stv PRIVATE PkgConfig::MPV)
target_link_libraries(stv PRIVATE PkgConfig::SDL3)
target_link_libraries(stv PRIVATE PkgConfig::SQLite3)
target_link_libraries(stv_migration PRIVATE PkgConfig::SQLite3)
