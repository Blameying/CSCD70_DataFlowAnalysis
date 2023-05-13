cmake_minimum_required(VERSION 3.18.4)
project(DFA)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

execute_process(
  COMMAND llvm-config --cmakedir
  OUTPUT_VARIABLE CMAKE_PREFIX_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
  COMMAND llvm-config --cxxflags
  OUTPUT_VARIABLE LLVM_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND llvm-config --includedir
  OUTPUT_VARIABLE LLVM_INCLUDEDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE)

find_package(LLVM 15 REQUIRED CONFIG)

if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

set(CMAKE_CXX_FLAGS "${LLVM_CXXFLAGS} -Wall")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

#if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set(CMAKE_CXX_CLANG_TIDY
#      "clang-tidy;-header-filter=.*;--extra-arg-before=-stdlib=libc++"
#  )
#  message(STATUS "Enabling clang-tidy")
#endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)

include(CTest)
enable_testing()
add_subdirectory(test)
