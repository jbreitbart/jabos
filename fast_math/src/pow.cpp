#include <stdint.h>
#include <iostream>
#include <cmath>
#include "fast_math.h"

double fast_pow (const double a, const double b) {
	if (a==1.0) return 1.0;
	int32_t tmp = (*(1 + (int32_t *)&a));
	int32_t tmp2 = (int32_t)(b * (tmp - 1072632447) + 1072632447);
	double p = 0.0;
	*(1 + (int32_t * )&p) = tmp2;

//	double re = std::pow (a, b);

// 	if ((std::abs(re - p) / p)*100 > 5)
// 		std::cerr << "pow error (" << a << ", " << b << "): " << re << " - " << p << std::endl;

	return p;
}
