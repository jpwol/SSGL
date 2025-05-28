cmake_minimum_required(VERSION 3.10)
project(SSGL)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS_RELEASE "-O2")

file(GLOB SRC_FILES "src/*.c")

add_library(SSGL SHARED ${SRC_FILES})
target_link_libraries(SSGL X11 Xext)
target_include_directories(SSGL PRIVATE include)

target_compile_options(SSGL PRIVATE -mavx2)
