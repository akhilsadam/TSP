#pragma once
#include <vector>
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
#include <variant>
#include <Way.h>
using way = std::variant<AddressList, Route>;
class Way
{
public:
	static double length(way w, int n = 0);
	static int size(way w);
	static void set(way w, int i, Address a);
	static way append(way w, Address x);
	static way insert(way w, int i, Address a);
	static const Address get(way w, int i);
	//static const std::vector<Address> get(way w, int start,int end);
	static way opt2(way w);

	static way greedy(way w);

	static way insert(way src, way sink, 
		int sink_insert,
		int src_start,
		int src_end);
	static way erase(way w, 
		int first,
		int last);
	static std::vector<Address>::iterator begin(way w);
	static void remove(way& w, int index);
	static int countPrimes(way& w,int v1, int v2);
	static way lapse(way w, double distance);
	static void print(way w);
};