#include "Path.h"
#include "AddressList.h"
#include <iostream>
#include <optional>
#include <cassert>

AddressList::AddressList() {}
AddressList::AddressList(std::vector<Address> in_path) : Path(in_path) {}

Address AddressList::last()
{
	if (std::size(path) == 0)
	{
		std::cout << "[ERROR] No last element in path of 0 length." << std::endl;
		throw("");
	}
	return path[std::size(path)-1];
}

std::string AddressList::display()
{
	std::string out = "";
	for (auto a : path)
	{
		out += a.display();
		out += ",";
	}
	return out;
}
void AddressList::print()
{
	std::cout << "PATH:[" << display() << "]" << std::endl;
}

std::vector<double> AddressList::x(double shift)
{
	std::vector<double> xs;
	for (auto a : path)
	{
		xs.push_back(a.x() + shift);
	}
	return xs;
}
std::vector<double> AddressList::y(double shift)
{
	std::vector<double> ys;
	for (auto a : path)
	{
		ys.push_back(a.y() + shift);
	}
	return ys;
}

AddressList AddressList::reverse_route(int m,int n, bool tour)
{
	int N = path.size() - 1;
	if (tour)
	{
		assert(m > 0); assert(m < n); assert(n < N);
	}
	else
	{
		assert(m >= 0); assert(m < n); assert(n <= N);
	}

	std::vector<Address> path2;
	path2.insert(path2.begin(), path.begin(), path.begin() + m);
	path2.insert(path2.begin() + m, path.rbegin() + (N - n), path.rbegin() + (N - m + 1));
	path2.insert(path2.begin() + (n+1), path.begin() + (n+1), path.end());

	assert(path2.size() == path.size());

	return path2;
}

AddressList AddressList::greedy_route()
{
	// Note using masking over deletion or copy (saves memory and avoids errors).
	int pathLen = path.size();
	Address current(0, 0);
	AddressList greed;
	std::optional<Address> opt;
	greed.add_address(index_closest_to(current));
	for(int i = 0; i < pathLen; i++)
	{
		opt = index_closest_to(current, greed, true);
		if (opt.has_value())
		{
			current = opt.value();
			//std::cout << opt.value().display();
			greed.add_address(current);
		}
		else
		{
			break;
		}
	}
	return greed;
}
AddressList AddressList::opt2_route(bool tour)
{
	int start = 0;
	AddressList best = *this;
	int N = size() - 1;

	if (tour)
	{
		best.add_address(Address());
		start++;
	}

	
	AddressList current;
	bool restart = false;
	while (true)
	{
		for (int n = start; n <= N; n++)
		{
			for (int m = start; m < n; m++)
			{
				current = best.reverse_route(m, n, tour);
				if (current.length() < best.length())
				{
					best = current;
					restart = true;
					break;
				}
			}
			if (restart)
			{
				break;
			}
		}
		//std::cout << restart << std::endl;
		if (!restart)
		{
			break;
		}
		restart = false;
	}

	if (tour)
	{
		best.remove_last();
	}

	return best;
}

