cmake_minimum_required(VERSION 3.10)
project(Sharp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES "src/*.c")

add_library(Sharp SHARED ${SRC_FILES})
target_link_libraries(Sharp X11 Xext)
target_include_directories(Sharp PRIVATE include)

target_compile_options(Sharp PRIVATE -mavx2)
