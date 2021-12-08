#include <vector>
#include <Address.h>
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
#include <variant>
#include <Way.h>
using way = std::variant<AddressList, Route>;
double Way::length(way w, int n)
{
	if (w.index() == 0)
	{
		if (n > 1)
		{
			return std::get<AddressList>(w).length(n);
		}
		return std::get<AddressList>(w).length();
	}

	if (n > 1)
	{
		return std::get<Route>(w).length(n);
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

way Way::insert(way w, int i, Address a)
{
	if (w.index() == 0)
	{
		auto x = std::get<AddressList>(w);
		x.insert(i, a);
		return x;
	}
	else
	{
		auto x = std::get<Route>(w);
		x.insert(i, a);
		return x;
	}
}


way Way::append(way w, Address x)
{
	if (w.index() == 0)
	{
		auto a = std::get<AddressList>(w);
		a.add_address(x);
		return a;
	}
	auto a = std::get<Route>(w);
	a.add_address(x);
	return a;
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

int Way::countPrimes(way& w, int v1, int v2)
{
	bool isAddressList = true;
	if (w.index() != 0)
	{
		isAddressList = false;
	}
	int count = 0;
	for (int i = v1; i < v2; i++)
	{
		if (isAddressList)
		{
			if (Address(std::get<AddressList>(w)[i]).isPrime()) count++;
		}
		else if (Address(std::get<Route>(w)[i]).isPrime())
		{
			count++;
		}
	}
	return count;
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

way Way::lapse(way w, double distance) // should break this up into smaller functions!
{
	bool isAddressList = true;
	if (w.index() != 0)
	{
		isAddressList = false;
	}

	double travelled = 0;
	double totravel = 0;
	int end = 1;
	int L = Way::size(w);

	if (L < 2) { return w; };

	bool foundLocation = false;

	if (!isAddressList) { L++; };

	for (int n = 2; n <= L; n++)
	{
		travelled = totravel;
		totravel = Way::length(w, n);
		end = n - 1;
		if (totravel > distance)
		{
			foundLocation = true;
			break;
		}
	}

	if (!foundLocation)
	{
		auto w2 = Way::erase(w, 0, Way::size(w)); return w2;
	}

	Address current;
	Address endpoint;

	if ((!isAddressList) && (end == L - 1))
	{
		//endpoint is depot (0,0) by default
	}
	else
	{
		endpoint = Way::get(w, end);
	}
	current = Way::get(w, end - 1);

	//weights
	double w1 = (distance - travelled) / (totravel - travelled);
	//L2 metric address creation
	double xf = w1 * endpoint.x() + (1.0 - w1) * current.x();
	double yf = w1 * endpoint.y() + (1.0 - w1) * current.y();

	Address location{ xf,yf };

	way w2 = Way::erase(w, 0, end);

	if (isAddressList)
	{
		auto a = std::get<AddressList>(w2);
		a.insert(0, location);
		return a;
	}
	else
	{
		auto a = static_cast<AddressList>(std::get<Route>(w2));
		a.insert(0, location);
		Address d;
		a.add_address(d);
		return a;
	}

}