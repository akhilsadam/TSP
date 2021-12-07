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

//way Way::insert(way sink, std::vector<Address>::iterator sbegin,
//									std::vector<Address>::iterator src_start,
//									std::vector<Address>::iterator src_end)
//{
//	way out = sink;
//	if (out.index() == 0)
//	{
//		std::get<AddressList>(out).insert(sbegin,src_start,src_end);
//	}
//	else
//	{
//		std::get<Route>(out).insert(sbegin, src_start, src_end);
//	}
//	return out;	
//}

way Way::insert(way src, way sink, int sbegin,int src_start,int src_end)
{
	way out = sink;
	bool isAddressList = true;
	if (out.index() != 0)
	{
		isAddressList = false;
	}

	for (int i = src_start; i < src_end; i++)
	{
		if (isAddressList)
		{
			std::get<AddressList>(out).insert(sbegin, Way::Way::get(src, i));
		}
		else 
		{
			std::get<Route>(out).insert(sbegin, Way::Way::get(src, i));
		}
	}
	return out;
}

way Way::erase(way w,int first, int last)
{
	way w2 = w;
	if (w2.index() == 0)
	{
		auto& a2 = std::get<AddressList>(w2);
		a2.erase(a2.begin()+first,a2.begin()+last);
	}
	else
	{
		auto& a2 = std::get<Route>(w2);
		a2.erase(a2.begin() + first, a2.begin() + last);
	}
	return w2;
}

void Way::remove(way& w, int index)
{
	if (w.index() == 0)
	{
		auto& a2 = std::get<AddressList>(w);
		a2.erase(a2.begin() + index);
	}
	else
	{
		auto& a2 = std::get<Route>(w);
		a2.erase(a2.begin() + index);
	}
}

void Way::print(way w)
{
	if (w.index() == 0)
	{
		auto a2 = std::get<AddressList>(w);
		a2.print();
	}
	else
	{
		auto a2 = std::get<Route>(w);
		a2.print();
	}
}
//way Way::erase(way w, int first, int last)
//{
//	way out = w;
//	bool isAddressList = true;
//	if (out.index() != 0)
//	{
//		isAddressList = false;
//	}
//
//	for (int i = first; i < last; i++)
//	{
//		if (isAddressList)
//		{
//			std::get<AddressList>(out).erase(i);
//		}
//		else
//		{
//			std::get<Route>(out).erase(i);
//		}
//	}
//	return out;
//}
std::vector<Address>::iterator Way::begin(way w)
{
	if (w.index() == 0)
	{
		return std::get<AddressList>(w).begin();
	}
	return std::get<Route>(w).begin();
}