#pragma once
#include <Options.h>
#include <vector>
#include "Address.h"
#include "Path.h"
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
#include <variant>
#include <Way.h>
#include <random>
using way = std::variant<AddressList, Route>;

/// <summary>
/// Using variant as it seems to be better than deriving from a virtual class,
/// since fully virtual classes (interfaces) are not allowed in C++, unfortunately.
/// (I would prefer if I had a set of methods with differing implementation
/// based on the object, and this seems better
/// than using the Path class, especially since g++ has issues with unimplemented virtual methods.)
/// </summary>

class MultiplePath
{
protected:
	std::vector<way> schedule; // would like to optimize this (eventually...)
public:
	MultiplePath();
	MultiplePath(std::vector<way> in_paths);
	MultiplePath(std::vector<AddressList> in_paths);
	MultiplePath(std::vector<Route> in_paths);

	int size();
	int n_deliveries();
	double length();
	way& operator[](int i);

	void removeDuplicates();
	void swap(int p1, int p2, int v1, int v2);	// single vertex swap, now obsolete
	void swap(int p1, int p2, int v11, int v12, int v21, int v22); // non-equal segment swap
	MultiplePath swapAndOptimize(int p1, int p2, int v11, int v12, int v21, int v22, bool opt2=true);
	MultiplePath opt2_heuristic(bool opt2=true);
	int randomPrime(double percentage, std::mt19937 rng_mt);
	void plot(std::string path="multiple.jpg");
	void print();

	std::vector<way> getSchedule();
};