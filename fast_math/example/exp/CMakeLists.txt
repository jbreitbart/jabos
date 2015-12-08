ADD_EXECUTABLE(exp exp.cpp)

TARGET_LINK_LIBRARIES(exp m fm)

# set minimum cmake version
cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

