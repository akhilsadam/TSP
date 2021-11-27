#pragma once
#include <assert.h>
#include <math.h>
#include "Options.h"

void assertion(double a, double b)
{
	//std::cout << fabs(a - b) << std::endl;
	//std::cout << powf(10, -1.0 * minprecision) << std::endl;
	assert(fabs(a - b) < powf(10,-1.0*minprecision));
}