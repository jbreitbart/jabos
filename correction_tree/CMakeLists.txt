# General project settings
project(correct_tree)
cmake_minimum_required(VERSION 2.8)

add_executable(main main.cpp correction_tree.c)

########
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX11_SUPPORTED)
if(CXX11_SUPPORTED)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # Using Clang
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "release")
    ADD_DEFINITIONS(-g -Ofast -march=native)
  endif()

  if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    ADD_DEFINITIONS(-g3 -O1 -Weverything)
    ADD_DEFINITIONS(-fno-omit-frame-pointer -fno-optimize-sibling-calls)
    ADD_DEFINITIONS(-fsanitize=address,undefined,integer -fno-sanitize=unsigned-integer-overflow)
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined,integer")
  endif()

  # Reduce the noise coming from Qt5
  ADD_DEFINITIONS(-Wno-inconsistent-missing-override -Wno-disabled-macro-expansion -Wno-deprecated)

  # Options required to reduce the noise of spdlog
  ADD_DEFINITIONS(-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-sign-conversion -Wno-padded -Wno-switch-enum)
  ADD_DEFINITIONS(-Wno-old-style-cast -Wno-undef -Wno-documentation-unknown-command)
  ADD_DEFINITIONS(-Wno-weak-vtables -Wno-global-constructors -Wno-exit-time-destructors -Wno-newline-eof)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Using GCC
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "release")
    ADD_DEFINITIONS(-g -O2 -march=native)
  endif()

  if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    ADD_DEFINITIONS(-g3 -O0 -Wall -Wextra -Wunused)
    ADD_DEFINITIONS(-fno-omit-frame-pointer)
  endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # Using Intel C++
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "release")
    ADD_DEFINITIONS(-g -fast)
  endif()

  if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    ADD_DEFINITIONS(-g3 -O0 -Wall -Wextra)
    ADD_DEFINITIONS(-fno-omit-frame-pointer)
  endif()
endif()
########
