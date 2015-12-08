#pragma once

#include "wavefront.h"

struct at {
 int counter;
 char buf[512 - sizeof(int)];
};

at done[512];

template <int block_size>
inline bool seidel_par_blocked_blocked_atomic (datatype ** A, const int size, const int steps) {
	
	datatype **cur = A;

	const int real_size = size-2;
	const int blocks = real_size / block_size;

	#pragma omp parallel
	{
		const int jump = omp_get_num_threads();
		const int id = omp_get_thread_num();
		
		for (int step = 0; step<steps; ++step) {
			
			#pragma omp for schedule(static,1)
			for (int i=0; i<blocks; ++i) {
				done[i].counter=0;
			}
			
			for (int x=id; x<blocks; x+=jump) {
					int y=0;
					int y_stop = 0;
					volatile int* ptr = &done[x-1].counter;
					
					while (y != blocks) {
						if (x!=0) {
							y_stop = *ptr;
						} else {
							y_stop=blocks;
						}

						for (; y<y_stop; ++y) {
							
							__sync_fetch_and_add(&done[x].counter, 1);
						}		

					}				
			}
			
			#pragma omp barrier
			
		}
	}
	
	
	return true;
}


#define CALL_PAR_BLOCKED_BLOCKED_ATOMIC(max) \
	\
	if (max <= size) { \
		gettimeofday(&tv1, NULL); \
		bool success = seidel_par_blocked_blocked_atomic<max> (A, size, steps); \
		gettimeofday(&tv2, NULL); \
		if (success) { \
			std::cout << "parallel " << t << " threads: " \
			          << timediff(tv2, tv1) \
			          << std::endl; \
			 \
			 \
		} \
	}



