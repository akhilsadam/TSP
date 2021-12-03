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
	static double length(way w);
	static int size(way w);
	static void set(way w, int i, Address a);
	static const Address get(way w, int i);
	//static const std::vector<Address> get(way w, int start,int end);
	static way opt2(way w);

	static way greedy(way w);

	static way insert(way sink, 
		std::vector<Address>::iterator sink_insert,
		std::vector<Address>::iterator src_start,
		std::vector<Address>::iterator src_end);
	static way erase(way w, 
		std::vector<Address>::iterator first,
		std::vector<Address>::iterator last);
	static std::vector<Address>::iterator begin(way w);
};