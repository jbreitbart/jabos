#include <iostream>
#include <omp.h>

#include "util.h"
#include "wavefront.h"

using namespace std;

int main (int argc, char *argv[]) {
	timeval tv1, tv2;
	
	const int size = (1<<12) + 2;
	const int steps = seidel_steps;
	cout << "******* SIZE: " << size << " *******" << endl;
	cout << endl;

	cout << "****** 2D Memory " << " ******" << endl;
	{
		datatype **A = new datatype*[size];
		datatype **A_copy = new datatype*[size];
		datatype **ref = new datatype*[size];
		
		
		for (int i=0; i<size; ++i) {
			A[i] = new datatype[size];
			A_copy[i] = new datatype[size];
			
			ref[i] = new datatype[size];
		}
		
		std::cout << "*********" << std::endl;

		
		for (int t=1; t<=max_threads; t*=2) {
			std::cout << "*********" << std::endl;
			
			omp_set_num_threads(t);

			CALL_PAR_BLOCKED_BLOCKED_ATOMIC(64)


			//caller_par_explicit_blocked_atomic<max_block_size> (A, ref, A_copy, size, seidel_steps, t);
		}

		for (int i=0; i<size; ++i) {
			delete[] A[i];
			delete[] A_copy[i];
			
			delete[] ref[i];
		}

		delete[] A;
		delete[] A_copy;
		delete[] ref;
	}
	
	return 0;
}
