cmake_minimum_required(VERSION 3.24)
project(secret-string-example VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_COLOR_DIAGNOSTICS ON)   # You might want to enable this (CMake 3.24+)

find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

add_subdirectory(./lib/secret-string)   # add secret-string target

add_executable(main main.cpp)
target_link_libraries(main PRIVATE fmt::fmt range-v3::range-v3 secret-string)
