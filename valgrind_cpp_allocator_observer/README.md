A small C++ sample showing how to use C++ as part of a valgrind tool.
This cannot be compiled stand-a-lone. Compile the C++ code with

  -std=c++11 -nostdlib -fno-exceptions -fno-unwind-tables

or it will not link correctly.
