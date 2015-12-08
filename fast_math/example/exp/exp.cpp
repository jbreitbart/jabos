#include <iostream>
#include <cmath>
#include <vector>
#include <sys/time.h>


#include "fast_math.h"

using namespace std;

double timediff(timeval tv2, timeval tv1) {
	return (double) (tv2.tv_sec - tv1.tv_sec) + ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000.0);
}

const int iter = 100;

int main() {
	timeval tv1, tv2;
	std::vector<double> a(iter);
	std::vector<double> b(iter);


	gettimeofday(&tv1, NULL);
	for (int i=0; i<iter; ++i) {
		a[i] = exp (i);
	}
	gettimeofday(&tv2, NULL);
	std::cout << "math.h: " << timediff(tv2, tv1) << std::endl;


	gettimeofday(&tv1, NULL);
	for (int i=0; i<iter; ++i) {
		b[i] = fast_exp (i);
	}
	gettimeofday(&tv2, NULL);
	std::cout << "fast_math.h: " << timediff(tv2, tv1) << std::endl;

	double max = 0;
	int max_i =iter*2;
	for (int i=0; i<iter; ++i) {
		const int index = i;
		if (max < std::abs(a[index] - b[index]) / a[index]) {
			max_i = index;
			max = std::abs(a[index] - b[index]) /a[index];
		}
	}

	cout << "max_i: " << max_i << endl;
	cout << "Max. % error: " << max*100 << endl;
	cout << "exp (" << max_i << ")" << endl;
	cout << "math.h: " << a[max_i] << endl;
	cout << "fast_math.h: " << b[max_i] << endl;

// 	cout << pow(2.3, 3) << " - " << fast_pow(2.3, 3) << endl;
}
