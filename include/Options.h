#pragma once
#include <limits>
#include <math.h>
//---------------- precision definition
// Not using Catch2 as that has undefined reference runtime errors on my machine
// using "half-precision" as minimum for testing, as the L2norm distance operation
// (sqrt of squared numbers) wipes out precision really easily.
constexpr double minprecision = std::numeric_limits<double>::digits10/2.0 -1;
//---------------- metric definition
#define L2norm // this is straight-line distance
//#define L1norm // this is Manhattan distance
//#define maxnorm // this is max(dx,dy) distance
//---------------- graph
constexpr double legend_space = 0.05; // legend spacing
constexpr int printDigits = 3; // # of digits to print in output
constexpr int printDecimalDigits = 2;
constexpr int textwidth = 60;
