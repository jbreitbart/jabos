/*
 * Copyright: See LICENSE file that comes with this distribution
 *
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
// #include <xmmintrin.h>
#include <math.h>



// prototypes

/**
 * In an external file as it must be compiled with -fno-strict-aliasing
 */
double fast_pow(double a, double b);
static inline double fast_exp (const double y);


// implementation

static const int    exp_c        = 60801;
static const double exp_a_double = 1048576/M_LN2;
// static const double exp_a_int    = 1512775;


static inline double fast_exp (const double y) {
	union {
		double d;
		struct {
			#ifdef LITTLE_ENDIAN
				int32_t j, i;
			#else
				int32_t i, j;
			#endif
		} n;
	} exp;

	exp.n.i = exp_a_double*y + (1072693248 - exp_c);

//	double re = std::exp (y);
//	const int temp = (std::abs(re - exp.d) / exp.d)*100;

//	if ( temp > 5 || temp<0)
//		std::cerr << "exp error (" << y << "): " << re << " - " << exp.d << std::endl;

	return exp.d;
}


#ifdef REPLACE_NORMAL_MATH
#define pow fast_pow
#define exp fast_exp
#endif

