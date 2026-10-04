cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libcontext)

add_library(libcontext INTERFACE
  src/context.h
  src/context.cpp.h
  src/context_data.h)
target_include_directories(libcontext INTERFACE
  src)

add_executable(tests
  tests/main.cpp)
target_link_libraries(tests PRIVATE
  libcontext)

enable_testing()
add_test(NAME tests COMMAND tests)
