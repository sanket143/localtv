cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OUTPUT_LIB_DIR "${CMAKE_BINARY_DIR}/lib")

project(localtv CXX C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(MPV REQUIRED IMPORTED_TARGET mpv)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SQLite3 REQUIRED IMPORTED_TARGET sqlite3)

add_executable(localtv
  src/main.cpp
  src/app.cpp
  src/mpv_helpers.cpp
  src/db.cpp
)

add_executable(localtv_setup src/setup.cpp src/db.cpp)
add_executable(localtv_migration src/migrate.cpp src/db.cpp)
# add_library(std OBJECT carbon-src/std/print.cpp)

target_link_libraries(localtv PRIVATE PkgConfig::MPV)
target_link_libraries(localtv PRIVATE PkgConfig::SDL3)
target_link_libraries(localtv PRIVATE PkgConfig::SQLite3)
target_link_libraries(localtv_setup PRIVATE PkgConfig::SQLite3)
target_link_libraries(localtv_migration PRIVATE PkgConfig::SQLite3)
