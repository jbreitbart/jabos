cmake_minimum_required(VERSION 2.4)

PROJECT(log_o_matic)

# set the include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# if you don want the full compiler output, uncomment the following line
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -W -O2 -g)

# set library output path
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/)

# generate library
ADD_LIBRARY(log_o_matic src/database.cpp src/database_entry.cpp src/log.cpp)

# generate our example
ADD_EXECUTABLE(simple_example example/log_o_matic.cpp)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES(simple_example log_o_matic)

INSTALL(DIRECTORY include/log_o_matic DESTINATION include)
INSTALL(TARGETS log_o_matic ARCHIVE DESTINATION lib)

