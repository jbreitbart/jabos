SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/)

ADD_DEFINITIONS(-fno-strict-aliasing) # required for pow.cpp
ADD_LIBRARY(fm STATIC pow.cpp)


# set minimum cmake version
cmake_minimum_required(VERSION 2.4)
