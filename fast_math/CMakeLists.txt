PROJECT(fast_math)

# set the include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# if you don want the full compiler output, uncomment the following line
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -W -pedantic)               # warnings
ADD_DEFINITIONS(-O3) # optimization flags -march=native
# ADD_DEFINITIONS(-msse -mfpmath=sse)               # enable SSE
# ADD_DEFINITIONS(-g3)                              # debug flags
# ADD_DEFINITIONS(-DREPLACE_NORMAL_MATH)            # replace math.h calls with fast math

# build the library
SUBDIRS(src)

# build the examples
SUBDIRS(example)

# set minimum cmake version
cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
