ADD_EXECUTABLE(pow pow.cpp)

TARGET_LINK_LIBRARIES(pow m fm)

# set minimum cmake version
cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

