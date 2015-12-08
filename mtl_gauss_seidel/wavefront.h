#pragma once

typedef double datatype;

#define max_block_size 1024
#define min_block_size 4
#define max_threads 32
#define seidel_steps 10000 /*0*/

template <typename T>
inline datatype calc(T cur, const int i, const int j, const int size=0) {
	return (cur[i-1][j] + cur[i+1][j] + cur[i][j-1] + cur[i][j+1]) * 0.25;
}

template <typename T>
inline datatype calc(T *cur, const int i, const int j, const int size) {
	return (cur[(i-1)*size + j] + cur[(i+1)*size + j] + cur[i*size + j-1] + cur[i*size + j+1]) * 0.25;
}


#include <iostream>

#include "seidel_par_blocked_blocked_atomic.h"
