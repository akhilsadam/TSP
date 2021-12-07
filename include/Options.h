#pragma once
#include <limits>
#include <vector>
#include <string>
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
const double legend_space = 0.05; // legend spacing
const int printDigits = 5; // # of digits to print in output
const int printDecimalDigits = 4;
const int textwidth = 60;
const std::vector<std::string> colors = {"red","blue","green","cyan","violet","crimson","pink"};
const int scale = 1;
//---------------- tests
const int loopMax = 1;