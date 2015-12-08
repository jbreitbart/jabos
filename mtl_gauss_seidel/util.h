#include <sys/time.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

inline double timediff(timeval tv2, timeval tv1) {
	return (double) (tv2.tv_sec - tv1.tv_sec) + ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000.0);
}

template <typename datatype>
inline double memory_bandwidth (const int size, const int iterations, const double time, const int inner=5, const int outer=1) {
	double inner_data  = (size-2)*(size-2)*sizeof(datatype); // inner matrix data
	double border_data = (size-2)*4       *sizeof(datatype); // border matrix data
	double data_accessed_per_iteration = inner_data*inner + border_data*outer;
	double data_accessed = data_accessed_per_iteration * iterations;
	return data_accessed / time / 1024 / 1024; //data in megabytes
}

inline double g_flops (const int size, const int iterations, const double time) {
	double inner_elements = (size-2)*(size-2);
	double flops_per_element = 4+1;
	return inner_elements * flops_per_element * iterations / 1000 / 1000 / 1000 / time;
}


template <typename T>
inline void fill (T** mem, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size; ++i) {
		for(int j=0; j<size; ++j) {
			mem[i][j] = (rand() ) / static_cast<T>(RAND_MAX) + 1;
		}
	}
}

template <typename T>
inline void fill (T* mem, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size*size; ++i) {
		mem[i] = (rand() ) / static_cast<T>(RAND_MAX) + 1;
	}
}

template<>
inline void fill<int> (int** mem, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size; i++)  {
		for(int j=0; j<size; ++j) {
			mem[i][j] = static_cast<int>(rand() & 0xFF) + 1;
		}
	}
}

template<>
inline void fill<int> (int* mem, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size*size; i++)  {
		mem[i] = static_cast<int>(rand() & 0xFF) + 1;
	}
}

template <typename T>
inline void copy (T** mem, T** copy, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size; ++i) {
		for(int j=0; j<size; ++j) {
			copy[i][j] = mem[i][j];
		}
	}
}

template <typename T>
inline void copy (T* mem, T* copy, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size*size; ++i) {
		copy[i] = mem[i];
	}
}

template <typename T>
inline void fill_zero (T** mem, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size; i++)  {
		for(int j=0; j<size; ++j) {
			mem[i][j] = 0;
		}
	}
}

template <typename T>
inline void fill_zero (T* mem, const int size) {
	#pragma omp parallel for
	for(int i=0; i<size*size; i++)  {
		mem[i] = 0;
	}
}

template<typename T>
inline void print_matrix (T** A, const int size) {
	for (int i=0; i<size; ++i) {
		for (int j=0; j<size; ++j) {
			std::cout << A[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

template<typename T>
inline void print_matrix (T* A, const int size) {
	for (int i=0; i<size; ++i) {
		for (int j=0; j<size; ++j) {
			std::cout << A[i*size+j] << " ";
		}
		std::cout << std::endl;
	}
}

template<typename T>
inline void compare_matrix (T** A, T** B, const int size) {
	#pragma omp parallel for schedule(static)
	for (int i=0; i<size; ++i) {
		for (int j=0; j<size; ++j) {
			if (std::abs(A[i][j] - B[i][j]) >0.2) {
				std::cerr << "Wrong result! " << i << ", " << j << " -- " << A[i][j] << " - " << B[i][j] 
				          << " -- " << A[i][j]-B[i][j] << std::endl;
				throw "error";
			}
		}
	}
}

template<typename T>
inline void compare_matrix (T* A, T* B, const int size) {
	#pragma omp parallel for schedule(static)
	for (int i=0; i<size; ++i) {
		for (int j=0; j<size; ++j) {
			if (std::abs(A[i*size+j] - B[i*size+j]) > 0.1) {
				std::cerr << "Wrong result! " << i << ", " << j << " -- " << A[i*size+j] << " - " << B[i*size+j] 
				          << " -- " << A[i*size+j]-B[i*size+j] << std::endl;
				throw "error";
			}
		}
	}
}
