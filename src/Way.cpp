#include <vector>
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
#include <variant>
#include <Way.h>
using way = std::variant<AddressList, Route>;
double Way::length(way w)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w).length();
	}
	return std::get<Route>(w).length();
}

int Way::size(way w)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w).size();
	}
	return std::get<Route>(w).size();
}

const Address Way::get(way w,int i)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w)[i];
	}
	return std::get<Route>(w)[i];
}

void Way::set(way w, int i, Address a)
{
	if (w.index() == 0)
	{
		std::get<AddressList>(w).set(i,a);
	}
	else
	{
		std::get<Route>(w).set(i, a);
	}
}

way Way::opt2(way w)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w).opt2_route();
	}
	return std::get<Route>(w).opt2_route();
}

way Way::greedy(way w)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w).greedy_route();
	}
	return std::get<Route>(w).greedy_route();
}

way Way::insert(way sink, std::vector<Address>::iterator sbegin,
									std::vector<Address>::iterator src_start,
									std::vector<Address>::iterator src_end)
{
	way out = sink;
	if (out.index() == 0)
	{
		std::get<AddressList>(out).insert(sbegin,src_start,src_end);
	}
	else
	{
		std::get<Route>(out).insert(sbegin, src_start, src_end);
	}
	return out;	
}

way Way::erase(way w, std::vector<Address>::iterator first, std::vector<Address>::iterator last)
{
	way w2 = w;
	if (w2.index() == 0)
	{
		std::get<AddressList>(w2).erase(first,last);
	}
	else
	{
		std::get<Route>(w2).erase(first,last);
	}
	return w2;
}
std::vector<Address>::iterator Way::begin(way w)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w).begin();
	}
	return std::get<Route>(w).begin();
}